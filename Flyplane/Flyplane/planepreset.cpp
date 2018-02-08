#include "planepreset.h"
#include <iostream>
#include <fstream>
#include "assetloader.h"

void PlanePreset::load(std::string presetname)
{
	this->file = presetname;
	std::ifstream file(presetname);
	std::string str;
	
	//read name
	std::getline(file, str);
	name = str;

	//read model
	std::getline(file, str);
	model = str;
	AssetLoader::getLoader().loadModel(model, name);

	//read normalspeed
	std::getline(file, str);
	normalspeed = std::stod(str, NULL);

	//read boostspeed
	std::getline(file, str);
	boostspeed = std::stod(str, NULL);

	//read breakforce
	std::getline(file, str);
	breakforce = std::stod(str, NULL);

	//read turnrate
	std::getline(file, str);
	turnrate = std::stod(str, NULL);

	while (std::getline(file, str)) {
		glm::vec3 pos;
		pos.x = std::stod(str, NULL);
		std::getline(file, str);
		pos.y = std::stod(str, NULL);
		std::getline(file, str);
		pos.z = std::stod(str, NULL);
		wepPos.push_back(pos);
	}
}
