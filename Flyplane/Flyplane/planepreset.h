#pragma once
#include <string>
#include <vector>
#include "glm\glm.hpp"

class PlanePreset {
public:
	std::string name = "";
	std::string model = "";
	std::string file = "";
	double normalspeed, boostspeed, breakforce, acceleration, turnrate;
	std::vector<std::string> textureNames;
	std::vector<glm::vec3> wepPos;
	PlanePreset() {}
	~PlanePreset() {}
	void load(std::string presetname);
};