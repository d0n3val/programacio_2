#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"

j1PathFinding::j1PathFinding() : 
j1Module(), 
map(NULL), 
last_path(DEFAULT_PATH_LENGTH),
width(0), 
height(0)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before render is available
bool j1PathFinding::Awake(j1IniReader* conf)
{
	LOG("Init Pathfinding library");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1PathFinding::Start()
{
	LOG("Start pathfinding");
	bool ret = true;

	return ret;
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Set the map to do the pathfinding
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= width &&
			pos.y >= 0 && pos.y <= height);
}

bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while(item)
	{
		if(item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while(item)
	{
		if(item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

uint PathNode::FindWalkableAdjacents(PathList& list_to_fill, j1PathFinding* path_finder) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if(path_finder->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if(path_finder->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if(path_finder->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if(path_finder->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.count();
}

int PathNode::Score() const
{
	return g + h;
}

int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	//h = pos.DistanceManhattan(destination);
	h = pos.DistanceNoSqrt(destination);

	return g + h;
}

// Actual A* algorithm -----------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	int ret = -1;
	int iterations = 0;

	if(IsWalkable(origin) && IsWalkable(destination))
	{
		PathList open;
		PathList closed;
		PathList adjacent;

		// Start pushing the origin in the open list
		open.list.add(PathNode(0, 0, origin, NULL));

		// Iterate while we have open destinations to visit
		do
		{
			// Move the lowest score cell from open list to the closed list
			p2List_item<PathNode>* lowest = open.GetNodeLowestScore();
			p2List_item<PathNode>* node = closed.list.add(lowest->data);
			open.list.del(lowest);

			// If destination was added, we are done!
			if(node->data.pos == destination)
			{
				last_path.Clear();
				// Backtrack to create the final path
				const PathNode* path_node = &node->data;

				while(path_node)
				{
					last_path.PushBack(path_node->pos);
					path_node = path_node->parent;
				}

				last_path.Flip();
				ret = last_path.Count();
				LOG("Created path of %d steps in %d iterations", ret, iterations);
				break;
			}

			// Fill a list with all adjacent nodes
			adjacent.list.clear();
			node->data.FindWalkableAdjacents(adjacent, this);

			p2List_item<PathNode>* item = adjacent.list.start;
			for(; item; item = item->next)
			{
				if(closed.Find(item->data.pos) != NULL)
					continue;

				p2List_item<PathNode>* adjacent_in_open = open.Find(item->data.pos);
				if(adjacent_in_open == NULL)
				{
					item->data.CalculateF(destination);
					open.list.add(item->data);
				}
				else
				{
					if(adjacent_in_open->data.g > item->data.g + 1)
					{
						adjacent_in_open->data.parent = item->data.parent;
						adjacent_in_open->data.CalculateF(destination);
					}
				}
			}

			++iterations;
		} while(open.list.count() > 0);
	}

	return ret;
}

