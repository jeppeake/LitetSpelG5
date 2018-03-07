#include "buildingpreset.h"
#include "buildingpreset.h"
#include <iostream>
#include <fstream>
#include "assetloader.h"



void BuildingPreset::load(std::string path)
{
	this->file = path;
	std::ifstream file(path);
	std::string str;

	//read name
	std::getline(file, str);
	name = str;

	while (std::getline(file, str)) {
		if (str.compare("model") == 0) {
			std::getline(file, str);
			model = str;
			AssetLoader::getLoader().loadModel(model, name);
			continue;
		}
		if (str.compare("health") == 0) {
			std::getline(file, str);
			health = std::stod(str);
			continue;
		}
		if (str.compare("exploderadius") == 0) {
			std::getline(file, str);
			exploderadius = std::stod(str);
			continue;
		}
		if (str.compare("explodedamage") == 0) {
			std::getline(file, str);
			explodedamage = std::stod(str);
			continue;
		}
		if (str.compare("explode") == 0) {
			explode = true;
			continue;
		}
	}
}
