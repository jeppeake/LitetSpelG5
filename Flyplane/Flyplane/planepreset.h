#pragma once
#include <string>
#include <vector>
#include "glm\glm.hpp"

class PlanePreset {
public:
	std::string name = "";
	std::string model = "";
	std::string file = "";
	std::string description = "";
	double normalspeed, boostspeed, breakforce, acceleration, turnrate;
	std::vector<std::string> textureNames;
	std::vector<glm::vec3> wepPos;
	std::vector<glm::vec3> enginePos;
	float engineRadius;
	std::string weapon = "nan";
	std::vector<std::string> turretFiles;
	std::vector<glm::vec3> turretPositions;
	bool turretWeapon = false;
	PlanePreset() {}
	~PlanePreset() {}
	void load(std::string presetname);
};