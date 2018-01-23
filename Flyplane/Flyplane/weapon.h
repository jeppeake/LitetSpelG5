#pragma once
#include "equipment.h"
#include <entityx\entityx.h>

struct WeaponStats {
	//projectile model, sound, range?, ammo, speed, 
	WeaponStats(unsigned int ammo, unsigned int range, float speed, float falloff) : ammo(ammo), range(range), speed(speed), falloff(falloff) {}

	unsigned int ammo, range;
	float speed, falloff;
};