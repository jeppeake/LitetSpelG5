#pragma once
#include <entityx/entityx.h>
#include "heightmap.h"
class Map
{
	Heightmap terrain;
	std::vector<Model> models;
	bool initialized = false;
public:
	void load(const std::string &TerrainPath, const std::string &buildingPath, entityx::EntityManager &mgr);
	void render();
};