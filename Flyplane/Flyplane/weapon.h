#pragma once
#include "model.h"
#include <entityx\entityx.h>
#include "timer.h"

struct WeaponStats {
	//projectile model, sound, range?, ammo, speed, 
	WeaponStats(unsigned int ammo, unsigned int range, float speed, float falloff, float cooldown) : ammo(ammo), range(range), speed(speed), falloff(falloff), cooldown(cooldown) {}

	unsigned int ammo, range;
	float speed, falloff, cooldown;
};

struct Weapon {
	Weapon(WeaponStats* stats, Model* model, bool playerOwned, glm::vec3 offset) : stats(stats), model(model), playerOwned(playerOwned), offset(offset) { }
	WeaponStats* stats;
	Model* model;
	bool shouldFire = false;
	bool active = false;
	bool playerOwned;
	Timer timer;
	glm::vec3 offset = glm::vec3(1.0,-0.5,0.0);
};