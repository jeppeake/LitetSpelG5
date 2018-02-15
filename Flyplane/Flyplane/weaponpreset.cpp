#include "weaponpreset.h"
#include <fstream>
#include "assetloader.h"

void WeaponPreset::load(std::string presetname)
{
	file = presetname;

	std::ifstream f(presetname);
	std::string str;

	std::getline(f, str);
	name = str;

	while (std::getline(f, str)) {
		if (str.compare("model") == 0) {
			std::getline(f, str);
			model = str;
			AssetLoader::getLoader().loadModel(model, name);
			continue;
		}
		else if (str.compare("projmodel") == 0) {
			std::getline(f, str);
			projModel = str;
			AssetLoader::getLoader().loadModel(projModel, projModel);
			continue;
		}
		else if (str.compare("ammo") == 0) {
			std::getline(f, str);
			ammo = std::stoi(str, NULL);
			continue;
		}
		else if (str.compare("lifetime") == 0) {
			std::getline(f, str);
			lifetime = std::stoi(str, NULL);
			continue;
		}
		else if (str.compare("speed") == 0) {
			std::getline(f, str);
			speed = std::stod(str, NULL);
			continue;
		}
		else if (str.compare("mass") == 0) {
			std::getline(f, str);
			mass = std::stod(str, NULL);
			continue;
		}
		else if (str.compare("cooldown") == 0) {
			std::getline(f, str);
			cooldown = std::stod(str, NULL);
			continue;
		}
		else if (str.compare("turnrate") == 0) {
			std::getline(f, str);
			turnRate = std::stod(str, NULL);
			continue;
		}
		else if (str.compare("scale") == 0) {
			std::getline(f, str);
			scale = std::stod(str, NULL);
			continue;
		}
		else if (str.compare("projscale") == 0) {
			std::getline(f, str);
			projScale = std::stod(str, NULL);
			continue;
		}
		else if (str.compare("infammo") == 0) {
			std::getline(f, str);
			infAmmo = std::stoi(str, NULL);
			continue;
		}
		else if (str.compare("ismissile") == 0) {
			std::getline(f, str);
			isMissile = std::stoi(str, NULL);
			continue;
		}
		else if (str.compare("dissappear") == 0) {
			std::getline(f, str);
			dissappear = std::stoi(str, NULL);
			continue;
		}
		else if (str.compare("offset") == 0) {
			std::getline(f, str);
			extraOffset.x = std::stod(str, NULL);
			std::getline(f, str);
			extraOffset.y = std::stod(str, NULL);
			std::getline(f, str);
			extraOffset.z = std::stod(str, NULL);
			continue;
		}
		else if (str.compare("detonaterange") == 0) {
			std::getline(f, str);
			detonateRange = std::stod(str, NULL);
			continue;
		}
		else if (str.compare("exploderadius") == 0) {
			std::getline(f, str);
			explodeRadius = std::stod(str, NULL);
			continue;
		}
		else if (str.compare("damage") == 0) {
			std::getline(f, str);
			damage = std::stod(str, NULL);
			continue;
		}
	}
}

Weapon* WeaponPreset::getWeapon()
{
	WeaponStats stats(ammo, lifetime, speed, mass, cooldown, infAmmo, turnRate, detonateRange, explodeRadius, damage);
	Weapon wep(stats,AssetLoader::getLoader().getModel(name), AssetLoader::getLoader().getModel(projModel), extraOffset);
	return &wep;
}
