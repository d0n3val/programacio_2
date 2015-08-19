#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Map.h"
#include "base64/base64.h"
#include "zlib/include/zlib.h"
#include "trim.h"
#include "SDL/include/SDL.h"
#include "math.h"

#pragma comment( lib, "zlib/libx86/zdll.lib" )

j1Map::j1Map() : j1Module()
{
	name.create("map");
	map_loaded = false;
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(j1IniReader* conf)
{
	LOG("Loading Map");
	bool ret = true;
	folder.create(conf->GetString("folder", "maps/"));

	if(Load(conf->GetString("map", "")) == true)
	{
		map_loaded = true;
		App->render->SetBackgroundColor(data.background_color);

		int w, h;
		uchar* data;
		if(CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	return ret;
}

// Called each loop iteration
bool j1Map::PreUpdate()
{
	return true;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = NULL;

	while(item)
	{
		if(id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

TileType* TileSet::GetTileType(int id) const
{
	p2List_item<TileType*>* item = tile_types.start;
	int relative_id = id - firstgid;

	while(item)
	{
		if(item->data->id == relative_id)
			return item->data;
		item = item->next;
	}

	return NULL;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

int Properties::Get(const char* value, int default_value) const
{
	p2List_item<Property*>* item = list.start;

	while(item)
	{
		if(item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}

bool j1Map::Update(float dt)
{
	// Render all layers with draw > 0
	p2List_item<MapLayer*>* item;
	item = data.layers.start;

	for(item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if(layer->properties.Get("Draw", 1) == 0)
			continue;

		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if(tileset == NULL)
					continue;

				SDL_Rect r = tileset->GetTileRect(tile_id);
				p2Point<int> pos = MapToWorld(x, y);

				App->render->Blit(tileset->texture, pos.x + tileset->offset_x, pos.y + tileset->offset_y, &r);
				//LOG("Rendering tile id %d at %d,%d rect x%d,y%d,w%d,h%d", tile_id, pos_x, pos_y, r.x, r.y, r.w, r.h);
			}
		}
	}

	return true;
}

p2Point<int> j1Map::MapToWorld(int x, int y) const
{
	p2Point<int> ret;

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x-y) * (data.tile_width / 2);
		ret.y = (x+y) * (data.tile_height / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

p2Point<int> j1Map::WorldToMap(int x, int y) const
{
	p2Point<int> ret;

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		x -= 8; // Tileset margins
		y -= 16;
		ret.x = (x / (data.tile_width / 2) + (y / (data.tile_height / 2))) / 2;
		ret.y = (y / (data.tile_height / 2) - (x / (data.tile_width / 2))) / 2;
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

bool j1Map::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}

	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item2;
	item2 = data.layers.start;

	while(item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}

	data.layers.clear();
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2String tmp("%s%s", folder.c_str(), file_name);

	char* buf;
	int size = App->fs->Load(tmp, &buf);
	pugi::xml_parse_result result = map_file.load_buffer(buf, size);

	RELEASE(buf);

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		ret = LoadMap();
	}

	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();
		if(ret == true)
		{
			ret = LoadTileset(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetTerrains(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetTileTypes(tileset, set);
		}
		data.tilesets.add(set);
	}

	if(ret == true)
	{
		pugi::xml_node layer;

		for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
		{
			ret = LoadLayer(layer);
		}
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name, s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item2 = data.layers.start;
		while(item2 != NULL)
		{
			MapLayer* l = item2->data;
			LOG("Layer ----");
			LOG("name: %s", l->name);
			LOG("width: %d height: %d", l->width, l->height);
			item2 = item2->next;
		}
	}


	return ret;
}

// create walkability map
bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	p2List_item<MapLayer*>* item;
	item = data.layers.start;

	for(item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if(layer->properties.Get("Walkability", 1) == 0)
			continue;

		uchar* map = new uchar[layer->width*layer->height];

		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;
				map[i] = 1;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;
				if(tileset != NULL)
				{
					TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
						map[i] = ts->properties.Get("walkable", 1);
					}
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2String bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.length() > 0)
		{
			p2String red, green, blue;
			bg_color.sub_string(1, 2, red);
			bg_color.sub_string(3, 4, green);
			bg_color.sub_string(5, 6, blue);

			int v = 0;

			sscanf_s(red, "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green, "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue, "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2String orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTileset(pugi::xml_node& map, TileSet* set)
{
	bool ret = true;
	//pugi::xml_node map = map_file.child("map").child("tileset");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'tileset' tag.");
		ret = false;
	}
	else
	{
		set->firstgid = map.attribute("firstgid").as_int();

		if(map.attribute("source").empty() == false)
		{
			pugi::xml_parse_result result = tileset_file.load_file(PATH(folder, map.attribute("source").as_string()));

			if(result == NULL)
			{
				LOG("Could not load tileset xml file. pugi error: %s", result.description());
				ret = false;
			}
		}
	}

	return ret;
}


bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name = tileset_node.attribute("name").as_string();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder, image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadTilesetTerrains(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node terrains = tileset_node.child("terraintypes");

	if(terrains == NULL)
	{
		LOG("Map does not contain terrain type definitions");
		//LOG("Error parsing tileset xml file: Cannot find 'terraintypes' tag.");
		//ret = false;
	}
	else
	{
		pugi::xml_node terrain;

		for(terrain = terrains.child("terrain"); terrain; terrain = terrain.next_sibling("terrain"))
		{
			TerrainType terrain_type;
			terrain_type.name = terrain.attribute("name").as_string();
			terrain_type.tile = terrain.attribute("tile").as_int();
			set->terrain_types.add(terrain_type);
			//LOG("terrain %s on tile %d", terrain_type.name, terrain_type.tile);
		}
	}

	return ret;
}


bool j1Map::LoadTilesetTileTypes(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node tile;

	for(tile = tileset_node.child("tile"); tile; tile = tile.next_sibling("tile"))
	{
		TileType* tile_type = new TileType();
		tile_type->id = tile.attribute("id").as_int();
		char types[MID_STR];
		strncpy_s(types, tile.attribute("terrain").as_string(), MID_STR);
		char* tmp = NULL;
		char* item;
		item = strtok_s(types, ",", &tmp);
		int i = 0;

		while(item != NULL)
		{
			switch(i)
			{
				case 0:
				tile_type->top_left = &set->terrain_types[atoi(item)];
				break;

				case 1:
				tile_type->top_right = &set->terrain_types[atoi(item)];
				break;

				case 2:
				tile_type->bottom_left = &set->terrain_types[atoi(item)];
				break;

				case 3:
				tile_type->bottom_right = &set->terrain_types[atoi(item)];
				break;
			}

			item = strtok_s(NULL, ",", &tmp);
			++i;
		}

		LoadProperties(tile, tile_type->properties);

		//Load any properties found


		set->tile_types.add(tile_type);
		
		/*
		LOG("tile id %d on terrains: %s %s %s %s",
		tile_type->id,
		tile_type->top_left->name,
		tile_type->top_right->name,
		tile_type->bottom_left->name,
		tile_type->bottom_right->name);

		p2List_item<Property*>* it = tile_type->properties.list.start;

		while(it)
		{
			LOG("--> %i: %s", it->data->value, it->data->name);
			it = it->next;
		}
		*/
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node node)
{
	bool ret = true;
	MapLayer* layer = new MapLayer();
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	LoadProperties(node, layer->properties);
	LOG("layer %s width %d height %d", layer->name, layer->width, layer->height);
	pugi::xml_node layer_data = node.child("data");

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		// decode base 64 then unzip ...
		unsigned long buffer_len = 1 + strlen(layer_data.text().as_string());
		char* buffer = new char[buffer_len];
		strncpy_s(buffer, buffer_len, layer_data.text().as_string(), buffer_len);
		trim_inplace(buffer, "\n\r ");
		char* decoded;
		unsigned int decoded_len;
		decoded = b64_decode(buffer, &decoded_len);
		RELEASE_ARRAY(buffer);
		buffer_len = decoded_len * 20;
		buffer = new char[buffer_len];
		int result = uncompress((Bytef*)buffer, &buffer_len, (Bytef*)decoded, decoded_len);
		free(decoded);

		if(result != Z_OK)
		{
			LOG("Could not decompress layer data, error code %d", result);
			ret = false;
		}
		else
		{
			layer->data = new unsigned __int32[buffer_len];
			memcpy(layer->data, buffer, buffer_len);
		}

		RELEASE_ARRAY(buffer);
	}

	data.layers.add(layer);
	return ret;
}

// Load a group of properties from a node and fill a list with it
bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;
	pugi::xml_node data = node.child("properties");

	if(data != NULL)
	{
		pugi::xml_node prop;

		for(prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Property* p = new Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();

			properties.list.add(p);
		}
	}

	return ret;
}