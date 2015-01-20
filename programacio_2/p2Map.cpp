#include "p2Defs.h"
#include "p2Log.h"
#include "p2App.h"
#include "p2Map.h"
#include "base64/base64.h"
#include "zlib/include/zlib.h"
#include "trim.h"

#pragma comment( lib, "zlib/libx86/zdll.lib" )

p2Map::p2Map() : p2Module()
{
	strncpy(name, "map", SHORT_STR);
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
	Load(App->config.GetString("maps", "map", "desert.tmx"));

	return ret;
}

// Called each loop iteration
bool p2Map::PreUpdate()
{
	return true;
}

bool p2Map::Update(float dt)
{
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
		LOG("Could not load map xml file %s. pugi error: %s", path, result.description());
		ret = false;
	}

	if(ret == true)
		ret = LoadMap();
	
	if(ret == true)
		ret = LoadTileset();

	if(ret == true)
		ret = LoadTilesetDetails();
	
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
		LOG("width: %d height: %d", data.tileset.width, data.tileset.height);
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

		pugi::xml_parse_result result = tileset_file.load_file(PATH(folder, map.attribute("source").as_string()));

		if(result == NULL)
		{
			LOG("Could not load tileset xml file. pugi error: %s", result.description());
			ret = false;
		}
	}

	return ret;
}


bool p2Map::LoadTilesetDetails()
{
	bool ret = true;

	pugi::xml_node tileset = tileset_file.child("tileset");

	if(tileset == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'tileset' tag.");
		ret = false;
	}
	else
	{
		strncpy(data.tileset.name, tileset.attribute("name").as_string(), MID_STR);
		data.tileset.width = tileset.attribute("tilewidth").as_int();
		data.tileset.height = tileset.attribute("tileheight").as_int();
		data.tileset.margin = tileset.attribute("margin").as_int();
		data.tileset.spacing = tileset.attribute("spacing").as_int();
	}

	return ret;
}

bool p2Map::LoadTilesetImage()
{
	bool ret = true;

	pugi::xml_node image = tileset_file.child("tileset").child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		data.tileset.texture = App->tex->Load(PATH(folder, image.attribute("source").as_string()));
	}

	return ret;
}

bool p2Map::LoadTilesetTerrains()
{
	bool ret = true;

	pugi::xml_node terrains = tileset_file.child("tileset").child("terraintypes");

	if(terrains == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'terraintypes' tag.");
		ret = false;
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

	pugi::xml_node tileset = tileset_file.child("tileset");

	if(tileset == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'tileset' tag.");
		ret = false;
	}
	else
	{
		pugi::xml_node tile;

		for(tile = tileset.child("tile"); tile; tile = tile.next_sibling("tile"))
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
	}

	return ret;
}

bool p2Map::LoadLayer(pugi::xml_node node)
{
	bool ret = true;

	MapLayer layer;

	strncpy(layer.name, node.attribute("name").as_string(), MID_STR);
	layer.width = node.attribute("width").as_int();
	layer.height = node.attribute("height").as_int();

	LOG("layer %s width %d height %d", layer.name, layer.width, layer.height);

	pugi::xml_node layer_data = node.child("data");

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
	}
	else
	{
		// decode base 64 then unzip ...
		char buffer[8192];
		
		trim_copy(buffer, layer_data.text().as_string(), "\n\r ");

		char* decoded;
		unsigned int decoded_len;
		
		decoded = b64_decode( buffer, &decoded_len );

		unsigned long buffer_len = 8192;
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
			layer.data = new unsigned __int32[buffer_len];
			memcpy(layer.data, buffer, buffer_len);
		}
	}

	data.layers.add(layer);

	return ret;
}