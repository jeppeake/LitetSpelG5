#pragma once
#include <string>


class BuildingPreset {
private:
	
public:
	std::string name, model, file;
	double health = 100, exploderadius = 50, explodedamage = 100;
	bool explode = false;
	BuildingPreset() {}
	void load(std::string file);
};