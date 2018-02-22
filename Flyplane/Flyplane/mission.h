#pragma once
#include "glm\glm.hpp"
#include <string>
#include <vector>
#include "entityx\entityx.h"
#include "model.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "assetloader.h"

#define CONDITION_NONE 0
#define CONDITION_DESTROY 1
#define CONDITION_DEFEND 2

#define MISSIONTYPE_DEFEND 0
#define MISSIONTYPE_ATTACK 1
#define MISSIONTYPE_KILLALL 2

struct EnemyInfo {
	glm::vec3 pos;
	std::string loadoutFile;
};

struct HouseInfo {
	glm::vec3 pos;
	Model* model;
	unsigned int condition; //0 = nothing, 1 = destroy, 2 = defend
};

struct Mission {
	unsigned int iLeader = 0;
	bool formation = false;
	bool hasLeader = false;
	double time;
	double huntPlayerDist = 2000;
	double firingDistance = 500;
	std::vector<EnemyInfo> enemies;
	std::vector<HouseInfo> houses;
	std::string file;
	unsigned int type = MISSIONTYPE_DEFEND;
	std::string missiontext = "";

	void loadMission(std::string missionFile) {
		file = missionFile;

		std::ifstream f(missionFile);
		std::string str;

		std::getline(f, str);
		type = std::stoi(str);

		while (std::getline(f, str)) {
			if (str.compare("formation") == 0) {
				std::getline(f, str);
				int val = std::stoi(str);
				formation = val;
				continue;
			}
			if (str.compare("spawnenemy") == 0) {
				std::getline(f, str);
				std::stringstream ss;
				ss.str(str);
				EnemyInfo temp;
				ss >> temp.pos.x;
				ss >> temp.pos.y;
				ss >> temp.pos.z;
				std::getline(f, str);
				temp.loadoutFile = str;
				enemies.push_back(temp);
				continue;
			}
			if (str.compare("makeleader") == 0) {
				formation = true;
				iLeader = enemies.size() - 1;
				continue;
			}
			if (str.compare("spawnhouse") == 0) {
				std::getline(f, str);
				int random = std::stoi(str);
				std::getline(f, str);
				std::stringstream ss;
				ss.str(str);
				HouseInfo temp;
				ss >> temp.pos.x;
				ss >> temp.pos.z;
				if (random) {

				}
				std::getline(f, str);
				temp.model = AssetLoader::getLoader().getModel(str);
				std::getline(f, str);
				temp.condition = std::stoi(str);
				houses.push_back(temp);
				continue;
			}
			if (str.compare("settimer") == 0) {
				std::getline(f, str);
				time = std::stod(str);
				continue;
			}
			if (str.compare("huntplayerdist") == 0) {
				std::getline(f, str);
				huntPlayerDist = std::stod(str);
				continue;
			}
			if (str.compare("firingdistance") == 0) {
				std::getline(f, str);
				firingDistance = std::stod(str);
				continue;
			}
			if (str.compare("text") == 0) {
				std::getline(f, str);
				missiontext = str;
			}
		}
	}
};