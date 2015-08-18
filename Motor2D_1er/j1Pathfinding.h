#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2DynArray.h"

#define  DEFAULT_PATH_LENGTH 50

class j1PathFinding : public j1Module
{
public:

	j1PathFinding();

	// Destructor
	virtual ~j1PathFinding();

	// Called before render is available
	bool Awake(j1IniReader* conf);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Set Map
	void SetMap(uint width, uint height, uchar* data);

	int CreatePath(const p2Point<int>& origin, const p2Point<int>& destination);

	const p2DynArray<p2Point<int>> GetLastPath() const;

	bool CheckBoundaries(const p2Point<int>& pos) const;

public:
	bool debug;

private:

	uint width;
	uint height;
	uchar* map;
	SDL_Texture* debug_tex;
	p2DynArray<p2Point<int>> last_path;
};


#endif // __j1PATHFINDING_H__