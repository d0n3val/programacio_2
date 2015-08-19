#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// ----------------------------------------------------
struct TerrainType
{
	p2String name;
	int	tile;
};

// ----------------------------------------------------
struct Property
{
	p2String name;
	int value;
};

// ----------------------------------------------------
struct Properties
{
	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while(item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	p2List<Property*>	list;
};

// ----------------------------------------------------
struct TileType
{
	int id;
	TerrainType* top_left;
	TerrainType* top_right;
	TerrainType* bottom_left;
	TerrainType* bottom_right;
	Properties properties;
};

// ----------------------------------------------------
struct TileSet
{
	~TileSet()
	{
		terrain_types.clear();

		p2List_item<TileType*>* item;
		item = tile_types.start;

		while(item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		tile_types.clear();
	}

	TileType* GetTileType(int id) const;
	SDL_Rect GetTileRect(int id) const;

	p2String			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
	p2List<TerrainType>	terrain_types;
	p2List<TileType*>	tile_types;
};

// ----------------------------------------------------
struct MapLayer
{
	p2String			name;
	Properties			properties;
	int					width;
	int					height;
	unsigned __int32*	data;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline unsigned __int32 Get(int x, int y)
	{
		return data[(y*width) + x];
	}
};

// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	//TileSet				tileset;
	p2List<MapLayer*>	layers;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(j1IniReader* conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	TileSet* GetTilesetFromTileId(int id) const;

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

private:

	bool LoadMap();
	bool LoadTileset(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetTerrains(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetTileTypes(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node node);
	bool LoadProperties(pugi::xml_node& node, Properties& list);

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	pugi::xml_document	tileset_file;

	p2String			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__