#pragma once
#include "model.h"
#include <entityx\entityx.h>
#include "timer.h"

struct WeaponStats {
	//projectile model, sound, range?, ammo, speed, 
	WeaponStats(unsigned int ammo, unsigned int range, float speed, float falloff, float cooldown, bool infAmmo) : ammo(ammo), range(range), speed(speed), falloff(falloff), cooldown(cooldown), infAmmo(infAmmo) {}

	unsigned int ammo, range;
	float speed, falloff, cooldown;
	bool infAmmo;
};

struct Weapon {
	Weapon(WeaponStats* stats, Model* model, Model* projectileModel, glm::vec3 offset) : stats(stats), model(model), projectileModel(projectileModel), offset(offset) { }
	WeaponStats* stats;
	Model* model;
	Model* projectileModel;
	bool shouldFire = false;
	bool active = false;
	Timer timer;
	glm::vec3 offset = glm::vec3(1.0,-0.5,0.0);
};