#pragma once
#include <string>
#include <vector>
#include "glm\glm.hpp"

#define SLOTLIMIT_LIGHT 1
#define SLOTLIMIT_MEDIUM 2
#define SLOTLIMIT_HEAVY 3
#define SLOTLIMIT_MASSIVE 4

class PlanePreset {
public:
	std::string name = "";
	std::string model = "";
	std::string file = "";
	std::string description = "";
	double normalspeed, boostspeed, breakforce, acceleration, turnrate;
	std::vector<std::string> textureNames;
	std::vector<glm::vec3> wepPos;
	std::vector<unsigned int> slotLimits;
	std::vector<glm::vec3> enginePos;
	std::vector<glm::vec3> wingTrailPos;
	float engineRadius;
	std::string weapon = "nan";
	std::vector<std::string> turretFiles;
	std::vector<glm::vec3> turretPositions;
	std::vector<glm::vec3> turretOrientations;
	std::vector<glm::vec3> turretFronts;
	bool turretWeapon = false;
	bool preorder = false;
	PlanePreset() {}
	~PlanePreset() {}
	void load(std::string presetname);
};