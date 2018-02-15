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

	while (std::getline(file, str)) {
		if (str.compare("model") == 0) {
			std::getline(file, str);
			model = str;
			AssetLoader::getLoader().loadModel(model, name);
			continue;
		}
		else if (str.compare("normalspeed") == 0) {
			std::getline(file, str);
			normalspeed = std::stod(str, NULL);
			continue;
		}
		else if (str.compare("boostspeed") == 0) {
			std::getline(file, str);
			boostspeed = std::stod(str, NULL);
			continue;
		}
		else if (str.compare("breakforce") == 0) {
			std::getline(file, str);
			breakforce = std::stod(str, NULL);
			continue;
		}
		else if (str.compare("acceleration") == 0) {
			std::getline(file, str);
			acceleration = std::stod(str, NULL);
			continue;
		}
		else if (str.compare("turnrate") == 0) {
			std::getline(file, str);
			turnrate = std::stod(str, NULL);
			continue;
		}
		else if (str.compare("skin") == 0) {
			std::getline(file, str);
			textureNames.push_back(str);
			std::string skinname = str;
			std::getline(file, str);
			AssetLoader::getLoader().loadTexture(str, skinname);
			std::cout << "Loaded texture: " << skinname << ", " << str << "\n";
			continue;
		}
		else if (str.compare("weppos") == 0) {
			std::getline(file, str);
			glm::vec3 pos;
			pos.x = std::stod(str, NULL);
			std::getline(file, str);
			pos.y = std::stod(str, NULL);
			std::getline(file, str);
			pos.z = std::stod(str, NULL);
			wepPos.push_back(pos);
			continue;
		}
	}
}
