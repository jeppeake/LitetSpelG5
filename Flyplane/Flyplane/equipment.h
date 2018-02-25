#pragma once
#include "weapon.h"

struct Equipment {
	//projectile model, sound, range?, ammo, speed, 
	Equipment(std::vector<Weapon> primary, std::vector<Weapon> special) : primary(primary), special(special) {}

	unsigned int selected = 0;
	std::vector<Weapon> primary;
	std::vector<Weapon> special;

	void addSpecialWeapon(Weapon weapon) {
		std::cout << "size of special: " << special.size() << std::endl;
		//if (special.size() == 0)
		selected = special.size() - 1;
		special.push_back(weapon);
		//special[0] = weapon;
		//selected = 0;
	}
};