#include "p2Defs.h"
#include "p2Log.h"
#include "p2App.h"
#include "p2Map.h"
#include "base64/base64.h"
#include "zlib/include/zlib.h"
#include "trim.h"
#include "SDL/include/SDL.h"
#include "math.h"

#pragma comment( lib, "zlib/libx86/zdll.lib" )

p2Map::p2Map() : p2Module()
{
	strncpy(name, "map", SHORT_STR);
	map_loaded = false;
}

// Destructor
p2Map::~p2Map()
{
}

// Called before render is available
bool p2Map::Awake()
{
	LOG("Loading Map");
	bool ret = true;

	strncpy(folder, App->config.GetString("maps", "folder", "maps/"), MID_STR);

	if(Load(App->config.GetString("maps", "map", "desert.tmx")) == true)
	{
		map_loaded = true;
		App->render->SetBackgroundColor(data.background_color);
	}

	return ret;
}

// Called each loop iteration
bool p2Map::PreUpdate()
{
	return true;
}

SDL_Rect p2Map::GetTileRect(int id)
{
	int relative_id = id - data.tileset.firstgid;
	SDL_Rect rect;

	rect.w = data.tileset.tile_width;
	rect.h = data.tileset.tile_height;

	rect.x = data.tileset.margin + ((rect.w + data.tileset.spacing) * (relative_id % data.tileset.num_tiles_width));
	rect.y = data.tileset.margin + ((rect.h + data.tileset.spacing) * (relative_id / data.tileset.num_tiles_width));

	return rect;
}

bool p2Map::Update(float dt)
{
	// Render all layers
	p2list_item<MapLayer*>* item;

	item = data.layers.start;
	while( item != NULL )
	{
		MapLayer* layer = item->data;

		int tile_half_width = data.tile_width / 2;
		int tile_half_height = data.tile_height / 2;

		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->Get(x,y);
				
				if(tile_id < data.tileset.firstgid)
					continue;

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

				App->render->Blit(data.tileset.texture, pos_x + data.tileset.offset_x, pos_y + data.tileset.offset_y, &r);
				
				//LOG("Rendering tile id %d at %d,%d rect x%d,y%d,w%d,h%d", tile_id, pos_x, pos_y, r.x, r.y, r.w, r.h);
			}
		}
		item = item->next;
	}

	return true;
}


bool p2Map::PostUpdate()
{
	return true;
}

// Called before quitting
bool p2Map::CleanUp()
{
	LOG("Unloading map");
	
	return true;
}

// Load new map
bool p2Map::Load(const char* path)
{
	bool ret = true;

	pugi::xml_parse_result result = map_file.load_file(PATH(folder, path));

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", PATH(folder, path), result.description());
		ret = false;
	}

	if(ret == true)
		ret = LoadMap();
	
	if(ret == true)
		ret = LoadTileset();

	if(ret == true)
		ret = LoadTilesetDetails();

	if(ret == true)
		ret = LoadTilesetImage();

	if(ret == true)
		ret = LoadTilesetTerrains();
	
	if(ret == true)
		ret = LoadTilesetTileTypes();

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
bool p2Map::LoadMap()
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

		char buf[SHORT_STR];
		strncpy(buf, map.attribute("backgroundcolor").as_string(), SHORT_STR);

		if(strlen(buf) > 0)
		{
			// #FFA500 so set r=0xFF, g=0xA5, b=0x00, a=0xFF.

			char r[3], g[3], b[3];

			sprintf(r, "%c%c", buf[1], buf[2]);
			sprintf(g, "%c%c", buf[3], buf[4]);
			sprintf(b, "%c%c", buf[5], buf[6]);
			
			sscanf(r, "%x", &data.background_color.r);
			sscanf(g, "%x", &data.background_color.g);
			sscanf(b, "%x", &data.background_color.b);

			data.background_color.a = 0;
		}
		else
		{
			data.background_color.r = 0;
			data.background_color.g = 0;
			data.background_color.b = 0;
			data.background_color.a = 0;
		}

		strncpy(buf, map.attribute("orientation").as_string(), SHORT_STR);

		if(strncmp(buf, "orthogonal", SHORT_STR) == 0)
			data.type = MAPTYPE_ORTHOGONAL;
		else if(strncmp(buf, "isometric", SHORT_STR) == 0)
			data.type = MAPTYPE_ISOMETRIC;
		else if(strncmp(buf, "staggered", SHORT_STR) == 0)
			data.type = MAPTYPE_STAGGERED;
		else
			data.type = MAPTYPE_UNKNOWN;
	}

	return ret;
}

bool p2Map::LoadTileset()
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
			tileset_node = map_file.child("map").child("tileset");

		if(tileset_node == NULL)
		{
			LOG("Error parsing tileset xml file: Cannot find 'tileset' tag.");
			ret = false;
		}
	}

	return ret;
}


bool p2Map::LoadTilesetDetails()
{
	bool ret = true;

	strncpy(data.tileset.name, tileset_node.attribute("name").as_string(), MID_STR);
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

bool p2Map::LoadTilesetImage()
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
		
		int w,h;
		SDL_QueryTexture(data.tileset.texture, NULL, NULL, &w, &h);
		
		data.tileset.tex_width = image.attribute("width").as_int();

		if(data.tileset.tex_width <= 0)
			data.tileset.tex_width = w;

		data.tileset.tex_height = image.attribute("height").as_int();

		if(data.tileset.tex_height <= 0)
			data.tileset.tex_height = h;
		
		data.tileset.num_tiles_width = data.tileset.tex_width / data.tileset.tile_width;
		data.tileset.num_tiles_height = data.tileset.tex_height / data.tileset.tile_height;
	}

	return ret;
}

bool p2Map::LoadTilesetTerrains()
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

			strncpy(terrain_type.name, terrain.attribute("name").as_string(), MID_STR);
			terrain_type.tile = terrain.attribute("tile").as_int();

			data.tileset.terrain_types.add(terrain_type);

			//LOG("terrain %s on tile %d", terrain_type.name, terrain_type.tile);
		}
	}

	return ret;
}


bool p2Map::LoadTilesetTileTypes()
{
	bool ret = true;

	pugi::xml_node tile;

	for(tile = tileset_node.child("tile"); tile; tile = tile.next_sibling("tile"))
	{
		TileType tile_type;

		tile_type.id = tile.attribute("id").as_int();
			
		char types[MID_STR];
		strncpy(types, tile.attribute("terrain").as_string(), MID_STR);
			
		char* item;
		item = strtok (types,",");
		int i = 0;
		while (item != NULL)
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
			item = strtok(NULL, ",");
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

bool p2Map::LoadLayer(pugi::xml_node node)
{
	bool ret = true;

	MapLayer *layer = new MapLayer();

	strncpy(layer->name, node.attribute("name").as_string(), MID_STR);
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
		char buffer[16384];
		
		trim_copy(buffer, layer_data.text().as_string(), "\n\r ");

		char* decoded;
		unsigned int decoded_len;
		
		decoded = b64_decode( buffer, &decoded_len );

		unsigned long buffer_len = 16384;
		unsigned int lenght_data;

		int result = uncompress((Bytef *) buffer, &buffer_len, (Bytef *)decoded, decoded_len);

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
	}

	data.layers.add(layer);

	return ret;
}