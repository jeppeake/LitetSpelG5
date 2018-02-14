#include "weaponpreset.h"
#include <fstream>
#include "assetloader.h"

void WeaponPreset::load(std::string presetname)
{
	file = presetname;

	std::ifstream f(presetname);
	std::string str;

	//read name
	std::getline(f, str);
	name = str;

	//read model
	std::getline(f, str);
	model = str;
	AssetLoader::getLoader().loadModel(model, name);

	//read projmodel
	std::getline(f, str);
	projModel = str;
	AssetLoader::getLoader().loadModel(projModel, projModel);

	//read int values
	std::getline(f, str);
	ammo = std::stoi(str, NULL);

	std::getline(f, str);
	lifetime = std::stoi(str, NULL);

	//read double values
	std::getline(f, str);
	speed = std::stod(str, NULL);

	std::getline(f, str);
	mass = std::stod(str, NULL);

	std::getline(f, str);
	cooldown = std::stod(str, NULL);

	std::getline(f, str);
	turnRate = std::stod(str, NULL);

	std::getline(f, str);
	scale = std::stod(str, NULL);

	std::getline(f, str);
	projScale = std::stod(str, NULL);

	//read bool values
	std::getline(f, str);
	infAmmo = std::stoi(str, NULL);

	std::getline(f, str);
	isMissile = std::stoi(str, NULL);

	std::getline(f, str);
	dissappear = std::stoi(str, NULL);

	std::getline(f, str);
	extraOffset.x = std::stod(str, NULL);
	std::getline(f, str);
	extraOffset.y = std::stod(str, NULL);
	std::getline(f, str);
	extraOffset.z = std::stod(str, NULL);

	std::getline(f, str);
	detonateRange = std::stod(str, NULL);
	std::getline(f, str);
	explodeRadius = std::stod(str, NULL);
	std::getline(f, str);
	explodeDamage = std::stod(str, NULL);
}
