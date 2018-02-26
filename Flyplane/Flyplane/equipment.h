#pragma once
#include "weapon.h"
#include "turret.h"

struct WeaponSlot {
	glm::vec3 pos;
	bool used;
	int slotNumber;
};

struct Equipment {
	//projectile model, sound, range?, ammo, speed, 
	Equipment(std::vector<Weapon> primary, std::vector<Weapon> special) : primary(primary), special(special) {}
	Equipment(std::vector<Weapon> primary, std::vector<Weapon> special, std::vector<Turret> turrets, std::vector<glm::vec3> weaponPos) : primary(primary), turrets(turrets), special(special) {
		for (int i = 0; i < weaponPos.size(); i++) {
			weaponSlots.push_back({ weaponPos[i], true, i });
		}
	}

	Equipment(std::vector<Weapon> primary, std::vector<Weapon> special, std::vector<Turret> turrets) : primary(primary), special(special), turrets(turrets) {}
	unsigned int selected = 0;
	std::vector<Weapon> primary;
	std::vector<Weapon> special;
	std::vector<WeaponSlot> weaponSlots;

	void addSpecialWeapon(Weapon weapon) {
		//std::cout << "size of special: " << special.size() << std::endl;
		//if (special.size() == 0)
		if (special.size() < weaponSlots.size()) {
			for (int i = 0; i < weaponSlots.size(); i++) {
				if (weaponSlots[i].used == false) {
					selected = special.size();
					weaponSlots[i].used = true;
					weaponSlots[i].slotNumber = selected;
					weapon.offset = weaponSlots[i].pos;
					special.push_back(weapon);

					std::cout << "added weapon to slot " << i << std::endl;

					break;
				}
				else if (i == weaponSlots.size() - 1) {
					std::cout << "couldnt find a free weapon slot" << std::endl;
				}
			}
		}
		else {
			std::cout << "size of special isnt less than weaponSlots" << std::endl;
		}
		//special[0] = weapon;
		//selected = 0;
	}

	void removeSpecialWeapon() {
		special.erase(special.begin() + selected);

		for (int i = 0; i < weaponSlots.size(); i++) {
			if (weaponSlots[i].slotNumber == selected) {
				weaponSlots[i].used = false;
			}
		}
	}
	std::vector<Turret> turrets;
};