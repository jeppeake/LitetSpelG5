#pragma once
#include "weapon.h"
#include "turret.h"
struct Equipment {
	//projectile model, sound, range?, ammo, speed, 
	Equipment(std::vector<Weapon> primary, std::vector<Weapon> special) : primary(primary), special(special) {}
	Equipment(std::vector<Weapon> primary, std::vector<Weapon> special, std::vector<Turret> turrets) : primary(primary), special(special), turrets(turrets) {}
	unsigned int selected = 0;
	std::vector<Weapon> primary;
	std::vector<Weapon> special;
	std::vector<Turret> turrets;
};