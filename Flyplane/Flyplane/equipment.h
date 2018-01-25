#pragma once
#include "weapon.h"

struct Equipment {
	//projectile model, sound, range?, ammo, speed, 
	Equipment(std::vector<Weapon> slots) : slots(slots) {}

	unsigned int selected = 0;
	std::vector<Weapon> slots;
};