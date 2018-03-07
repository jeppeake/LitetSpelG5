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
			std::string skinname = str;
			std::getline(file, str);
			AssetLoader::getLoader().loadTexture(str, skinname);
			std::getline(file, str);
			bool preorder = std::stoi(str);
			if (preorder) {
				if(this->preorder)
					textureNames.push_back(skinname);
			}
			else {
				textureNames.push_back(skinname);
			}
				
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
		else if (str.compare("primaryweapon") == 0) {
			std::getline(file, str);
			weapon = str;
			continue;
		}
		else if (str.compare("turretweapon") == 0) {
			std::getline(file, str);
			turretWeapon = std::stoi(str, NULL);
			continue;
		}
		else if (str.compare("turret") == 0) {
			//turret config file
			std::getline(file, str);
			turretFiles.emplace_back(str);
			//turret position
			glm::vec3 Tpos;
			std::getline(file, str);
			Tpos.x = std::stod(str, NULL);
			std::getline(file, str);
			Tpos.y = std::stod(str, NULL);
			std::getline(file, str);
			Tpos.z = std::stod(str, NULL);
			turretPositions.push_back(Tpos);
			//turret orientation
			glm::vec3 Tori;
			std::getline(file, str);
			Tori.x = std::stod(str, NULL);
			std::getline(file, str);
			Tori.y = std::stod(str, NULL);
			std::getline(file, str);
			Tori.z = std::stod(str, NULL);
			turretOrientations.push_back(Tori);
			//turret front
			glm::vec3 Tfront;
			std::getline(file, str);
			Tfront.x = std::stod(str, NULL);
			std::getline(file, str);
			Tfront.y = std::stod(str, NULL);
			std::getline(file, str);
			Tfront.z = std::stod(str, NULL);
			turretFronts.push_back(Tfront);
			continue;
		}
		else if (str.compare("description") == 0) {
			std::getline(file, str);
			description = str;
		}
		else if (str.compare("enginepos") == 0) {
			glm::vec3 pos;

			std::getline(file, str);
			pos.x = std::stod(str, NULL);

			std::getline(file, str);
			pos.y = std::stod(str, NULL);

			std::getline(file, str);
			pos.z = std::stod(str, NULL);

			enginePos.push_back(pos);
		}
		else if (str.compare("engineradius") == 0) {
			std::getline(file, str);
			engineRadius = std::stod(str, NULL);
		}
		else if (str.compare("wingtrailpos") == 0) {
			glm::vec3 pos;

			std::getline(file, str);
			pos.x = std::stod(str, NULL);

			std::getline(file, str);
			pos.y = std::stod(str, NULL);

			std::getline(file, str);
			pos.z = std::stod(str, NULL);

			wingTrailPos.push_back(pos);
		}
	}
}
