#pragma once
#include "equipment.h"
#include "model.h"
#include <entityx\entityx.h>

struct WeaponStats {
	//projectile model, sound, range?, ammo, speed, 
	WeaponStats(unsigned int ammo, unsigned int range, float speed, float falloff) : ammo(ammo), range(range), speed(speed), falloff(falloff) {}

	unsigned int ammo, range;
	float speed, falloff;
};

struct Weapon {
	Weapon(WeaponStats* stats, Model* model, bool playerOwned) : stats(stats), model(model), playerOwned(playerOwned) { }
	WeaponStats* stats;
	Model* model;
	bool shouldFire = false;
	bool playerOwned;
};