#pragma once
#include <string>
#include <vector>
#include "glm\glm.hpp"
#include "turret.h"


class WeaponPreset {
public:
	std::string name;
	std::string file;
	std::string model;
	std::string projModel;
	unsigned int ammo, lifetime;
	double speed, mass, cooldown, turnRate, scale, projScale;
	bool infAmmo, isMissile, dissappear;
	double detonateRange, explodeRadius, damage, droptime = 1.0;
	glm::vec3 extraOffset = glm::vec3(0,0,0);
	WeaponPreset() {}
	~WeaponPreset() {}
	void load(std::string presetname);
	Weapon* getWeapon();
};

class TurretPreset {
public:
	std::string name;
	std::string file;
	std::string projModel;

	unsigned int ammo, lifetime;
	double speed, mass, cooldown, turnRate, scale, projScale;
	bool infAmmo, isMissile, dissappear;
	double detonateRange, explodeRadius, damage, droptime = 1.0;

	glm::vec3 offset = glm::vec3(0, 0, 0);
	glm::vec3 projectileOffset = glm::vec3(0, 0, 0);
	//glm::quat orientation = glm::quat(1, 0, 0, 0);
	//glm::vec3 front = glm::vec3(1.f, 0.f, 0.f);

	glm::vec2 elevationLimit = glm::vec2(90.f, 90.f);
	glm::vec2 traverseLimit = glm::vec2(90.f, 90.f);
	float range = 1000.f;
	std::string baseModel;
	std::string gunModel;

	bool autoFire = true;

	TurretPreset() {}
	~TurretPreset() {}
	void load(std::string presetname);
	Turret getTurret();
};