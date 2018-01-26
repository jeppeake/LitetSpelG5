#pragma once
#include "weapon.h"

struct Equipment {
	//projectile model, sound, range?, ammo, speed, 
	Equipment(std::vector<Weapon> primary, std::vector<Weapon> special) : primary(primary), special(special) {}

	unsigned int selected = 0;
	std::vector<Weapon> primary;
	std::vector<Weapon> special;
};