#include "map.h"
#include "renderer.h"
void Map::load(const std::string &TerrainPath, const std::string &buildingPath, entityx::EntityManager &mgr)
{
	heightmap.loadMap(TerrainPath, "assets/textures/grass.png");
	initialized = true;
	//Ladda in modeller f�r hus
	//Skapa entities f�r dessa hus
}
void Map::render()
{
	if (!initialized)
		return;
	//Renderer::getRenderer().addToList(&heightmap);
}
