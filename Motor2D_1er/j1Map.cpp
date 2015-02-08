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
bool j1Map::Awake()
{
	LOG("Loading Map");
	bool ret = true;
	folder.create(App->config.GetString("maps", "folder", "maps/"));

	if(Load(App->config.GetString("maps", "map", "")) == true)
	{
		map_loaded = true;
		App->render->SetBackgroundColor(data.background_color);
	}

	return ret;
}

// Called each loop iteration
bool j1Map::PreUpdate()
{
	return true;
}

SDL_Rect j1Map::GetTileRect(int id)
{
	int relative_id = id - data.tileset.firstgid;
	SDL_Rect rect;
	rect.w = data.tileset.tile_width;
	rect.h = data.tileset.tile_height;
	rect.x = data.tileset.margin + ((rect.w + data.tileset.spacing) * (relative_id % data.tileset.num_tiles_width));
	rect.y = data.tileset.margin + ((rect.h + data.tileset.spacing) * (relative_id / data.tileset.num_tiles_width));
	return rect;
}

bool j1Map::Update(float dt)
{
	// Render all layers
	p2List_item<MapLayer*>* item;
	item = data.layers.start;

	while(item != NULL)
	{
		MapLayer* layer = item->data;
		int tile_half_width = data.tile_width / 2;
		int tile_half_height = data.tile_height / 2;

		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->Get(x, y);

				if(tile_id < data.tileset.firstgid)
				{
					continue;
				}

				SDL_Rect r = GetTileRect(tile_id);
				int pos_x;
				int pos_y;

				if(data.type == MAPTYPE_ORTHOGONAL)
				{
					pos_x = (x * data.tile_width);
					pos_y = (y * data.tile_height);
				}
				else if(data.type == MAPTYPE_ISOMETRIC)
				{
					pos_x = (x * tile_half_width) - (y * tile_half_width);
					pos_y = (y * tile_half_height) + (x * tile_half_height);
				}
				else
				{
					LOG("Unknown map type");
					continue;
				}

				App->render->Blit(data.tileset.texture, pos_x + data.tileset.offset_x, pos_y + data.tileset.offset_y, &r);
				//LOG("Rendering tile id %d at %d,%d rect x%d,y%d,w%d,h%d", tile_id, pos_x, pos_y, r.x, r.y, r.w, r.h);
			}
		}

		item = item->next;
	}

	return true;
}


bool j1Map::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");
	data.tileset.terrain_types.clear();
	data.tileset.tile_types.clear();
	// Render all layers
	p2List_item<MapLayer*>* item;
	item = data.layers.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}

	data.layers.clear();
	return true;
}

// Load new map
bool j1Map::Load(p2String path)
{
	bool ret = true;
	p2String tmp("%s%s", folder.c_str(), path.c_str());

	pugi::xml_parse_result result = map_file.load_file(tmp);

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", tmp.c_str(), result.description());
		ret = false;
	}

	if(ret == true)
	{
		ret = LoadMap();
	}

	if(ret == true)
	{
		ret = LoadTileset();
	}

	if(ret == true)
	{
		ret = LoadTilesetDetails();
	}

	if(ret == true)
	{
		ret = LoadTilesetImage();
	}

	if(ret == true)
	{
		ret = LoadTilesetTerrains();
	}

	if(ret == true)
	{
		ret = LoadTilesetTileTypes();
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
		LOG("Successfully parsed map XML file: %s", path);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);
		LOG("Tileset ----");
		LOG("name: %s firstgid: %d", data.tileset.name, data.tileset.firstgid);
		LOG("tile width: %d tile height: %d", data.tileset.tile_width, data.tileset.tile_height);
		LOG("spacing: %d margin: %d", data.tileset.spacing, data.tileset.margin);
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

bool j1Map::LoadTileset()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map").child("tileset");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'tileset' tag.");
		ret = false;
	}
	else
	{
		data.tileset.firstgid = map.attribute("firstgid").as_int();

		if(map.attribute("source").empty() == false)
		{
			pugi::xml_parse_result result = tileset_file.load_file(PATH(folder, map.attribute("source").as_string()));

			if(result == NULL)
			{
				LOG("Could not load tileset xml file. pugi error: %s", result.description());
				ret = false;
			}

			tileset_node = tileset_file.child("tileset");
		}
		else
		{
			tileset_node = map_file.child("map").child("tileset");
		}

		if(tileset_node == NULL)
		{
			LOG("Error parsing tileset xml file: Cannot find 'tileset' tag.");
			ret = false;
		}
	}

	return ret;
}


bool j1Map::LoadTilesetDetails()
{
	bool ret = true;
	data.tileset.name = tileset_node.attribute("name").as_string();
	data.tileset.tile_width = tileset_node.attribute("tilewidth").as_int();
	data.tileset.tile_height = tileset_node.attribute("tileheight").as_int();
	data.tileset.margin = tileset_node.attribute("margin").as_int();
	data.tileset.spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		data.tileset.offset_x = offset.attribute("x").as_int();
		data.tileset.offset_y = offset.attribute("y").as_int();
	}
	else
	{
		data.tileset.offset_x = 0;
		data.tileset.offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage()
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
		data.tileset.texture = App->tex->Load(PATH(folder, image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(data.tileset.texture, NULL, NULL, &w, &h);
		data.tileset.tex_width = image.attribute("width").as_int();

		if(data.tileset.tex_width <= 0)
		{
			data.tileset.tex_width = w;
		}

		data.tileset.tex_height = image.attribute("height").as_int();

		if(data.tileset.tex_height <= 0)
		{
			data.tileset.tex_height = h;
		}

		data.tileset.num_tiles_width = data.tileset.tex_width / data.tileset.tile_width;
		data.tileset.num_tiles_height = data.tileset.tex_height / data.tileset.tile_height;
	}

	return ret;
}

bool j1Map::LoadTilesetTerrains()
{
	bool ret = true;
	pugi::xml_node terrains = tileset_node.child("terraintypes");

	if(terrains == NULL)
	{
		LOG("Map does not contain terrin type definitions");
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
			data.tileset.terrain_types.add(terrain_type);
			//LOG("terrain %s on tile %d", terrain_type.name, terrain_type.tile);
		}
	}

	return ret;
}


bool j1Map::LoadTilesetTileTypes()
{
	bool ret = true;
	pugi::xml_node tile;

	for(tile = tileset_node.child("tile"); tile; tile = tile.next_sibling("tile"))
	{
		TileType tile_type;
		tile_type.id = tile.attribute("id").as_int();
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
				tile_type.top_left = &data.tileset.terrain_types[atoi(item)];
				break;

				case 1:
				tile_type.top_right = &data.tileset.terrain_types[atoi(item)];
				break;

				case 2:
				tile_type.bottom_left = &data.tileset.terrain_types[atoi(item)];
				break;

				case 3:
				tile_type.bottom_right = &data.tileset.terrain_types[atoi(item)];
				break;
			}

			item = strtok_s(NULL, ",", &tmp);
			++i;
		}

		data.tileset.tile_types.add(tile_type);
		/*
		LOG("tile id %d on terrains: %s %s %s %s",
		tile_type.id,
		tile_type.top_left->name,
		tile_type.top_right->name,
		tile_type.bottom_left->name,
		tile_type.bottom_right->name);*/
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
		buffer_len = decoded_len * 10;
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