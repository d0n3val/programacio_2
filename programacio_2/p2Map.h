#ifndef __P2MAP_H__
    #define __P2MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2Textures.h"
#include "p2List.h"
#include "p2Module.h"

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct TerrainType
{
	char name[MID_STR];
	int	tile;
};

struct TileType
{
	int id;
	TerrainType* top_left;
	TerrainType* top_right;
	TerrainType* bottom_left;
	TerrainType* bottom_right;
};

struct TileSet
{
	char				name[MID_STR];
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
	p2list<TerrainType>	terrain_types;
	p2list<TileType>	tile_types;
};

struct MapLayer
{
	char name[MID_STR];
	int width;
	int height;
	unsigned __int32* data;

	MapLayer() : data(NULL)	{}
	~MapLayer() { /*LOG("release map data****");*/ RELEASE(data); }

	inline unsigned __int32 Get(int x, int y) { return data[(y*width)+x]; }
};

struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	TileSet				tileset;
	p2list<MapLayer *>	layers;
};


class p2Map : public p2Module
{
public:

	p2Map();

    // Destructor
    virtual ~p2Map();

    // Called before render is available
    bool Awake();

    // Called each loop iteration
    bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

    // Called before quitting
    bool CleanUp();

	// Load new map
	bool Load(const char* path);
	
	SDL_Rect GetTileRect(int id);

private:

	bool LoadMap();
	bool LoadTileset();
	bool LoadTilesetDetails();
	bool LoadTilesetImage();
	bool LoadTilesetTerrains();
	bool LoadTilesetTileTypes();
	bool LoadLayer(pugi::xml_node node);

public:

	MapData data;
	
private:

	pugi::xml_document	map_file;
	pugi::xml_document	tileset_file;
	pugi::xml_node		tileset_node;

	char				folder[MID_STR];
	bool				map_loaded;
};

#endif // __P2MAP_H__