#include "map.h"
#include "renderer.h"
void Map::load(const std::string &TerrainPath, const std::string &buildingPath, entityx::EntityManager &mgr)
{
	heightmap.loadMap(TerrainPath, "assets/textures/grass.png");
	initialized = true;
	//Ladda in modeller för hus
	//Skapa entities för dessa hus
}
void Map::render()
{
	if (!initialized)
		return;
	//Renderer::getRenderer().addToList(&heightmap);
}
