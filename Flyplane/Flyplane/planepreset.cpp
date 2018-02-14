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

	//read breakforce
	std::getline(file, str);
	acceleration = std::stod(str, NULL);

	//read turnrate
	std::getline(file, str);
	turnrate = std::stod(str, NULL);

	int texCount = 0;
	std::getline(file, str);
	while (str.compare("-end textures-") != 0) {
		textureNames.push_back(str);
		std::string texPath;
		std::getline(file, texPath);
		AssetLoader::getLoader().loadTexture(texPath, str);
		std::cout << "Loaded texture: " << str << ", " << texPath << "\n";
		std::getline(file, str);
	}

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
