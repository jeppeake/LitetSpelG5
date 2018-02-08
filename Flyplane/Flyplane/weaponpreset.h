#pragma once
#include <string>
#include <vector>
#include "glm\glm.hpp"

class WeaponPreset {
public:
	std::string name;
	std::string file;
	std::string model;
	std::string projModel;
	unsigned int ammo, lifetime;
	double speed, mass, cooldown, turnRate, scale, projScale;
	bool infAmmo, isMissile, dissappear;
	WeaponPreset() {}
	~WeaponPreset() {}
	void load(std::string presetname);
};