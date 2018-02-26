#include "assetloader.h"
#include "weaponpreset.h"

Text * AssetLoader::getText() &
{
	return text;
}

Text * AssetLoader::getMenutext() &
{
	return menutext;
}

Text * AssetLoader::getBigtext() &
{
	return bigtext;
}

Text * AssetLoader::getHighscoreText() &
{
	return highscoreText;
}

void AssetLoader::loadModel(const std::string &filename, const std::string &name) {
	auto find = models.find(name);
	if (find == models.end()) {
		//Model model;
		//models[name] = model;
		models[name].load(filename);
	}
}

void AssetLoader::loadTexture(const std::string filename, const std::string name) {
	auto find = textures.find(name);
	if (find == textures.end()) {
		//Texture texture;
		//textures[name] = texture;
		bool success = textures[name].loadTexture(filename);
	}
}

void AssetLoader::loadWeapon(const std::string filename, const std::string name) {
	auto find = weapons.find(name);
	if (find == weapons.end()) {
		WeaponPreset wp;
		wp.load(filename);

		AssetLoader::getLoader().loadModel(wp.model, wp.name);
		AssetLoader::getLoader().loadModel(wp.projModel, wp.projModel);

		WeaponStats stats = WeaponStats(wp.ammo, wp.lifetime, wp.speed, wp.mass, wp.cooldown, wp.infAmmo, wp.turnRate, wp.detonateRange, wp.explodeRadius, wp.damage, wp.droptime);

		std::cout << "name: " << wp.name << " modelPtr: " << AssetLoader::getLoader().getModel(wp.name) << std::endl;
		weapons[name] = Weapon(stats, AssetLoader::getLoader().getModel(wp.name), AssetLoader::getLoader().getModel(wp.projModel), wp.extraOffset, glm::vec3(wp.scale), glm::vec3(wp.projScale), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), wp.isMissile, wp.dissappear);//Weapon(WeaponStats(1, 100, 500, 10, 1, false, 3, 50, 60, 600, 1), AssetLoader::getLoader().getModel("fishrod"), AssetLoader::getLoader().getModel("fishrod"), glm::vec3(0, 0.0, 0.0), glm::vec3(0.6), glm::vec3(0.6), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), true, true);
	}
}

void AssetLoader::loadHeightmap(const std::string &maptxt, const std::string &name) {
	auto find = heightmaps.find(name);
	if (find == heightmaps.end()) {
		//Heightmap hmap;
		heightmaps[name].loadMap(maptxt);
	}
}

void AssetLoader::loadSound(const std::string &filename, const std::string &name) {
	auto find = sounds.find(name);
	if (find == sounds.end()) {
		//sf::SoundBuffer sb;
		//sounds[name] = sb;
		if (!sounds[name].loadFromFile(filename))
			std::cout << "AssetLoader error: sound " << filename << " couldn't load." << std::endl;
	}
}

Model* AssetLoader::getModel(const std::string name) {
	auto find = models.find(name);
	if (find != models.end())
		return &(find->second);
	else
		std::cout << "AssetLoader error: Could not find requested model: " << name << "\n";
	return nullptr;
}

Texture * AssetLoader::getTexture(const std::string name)
{
	auto find = textures.find(name);
	if (find != textures.end())
		return &(find->second);
	else
		std::cout << "AssetLoader error: Could not find requested model: " << name << "\n";
	return nullptr;
}

Heightmap* AssetLoader::getHeightmap(const std::string name) {
	auto find = heightmaps.find(name);
	if (find != heightmaps.end())
		return &(find->second);
	else
		std::cout << "AssetLoader error: Could not find requested heightmap." << "\n";
	return nullptr;
}

sf::SoundBuffer* AssetLoader::getSoundBuffer(const std::string name) {
	auto find = sounds.find(name);
	if (find != sounds.end())
		return &(find->second);
	else
		std::cout << "AssetLoader error: Could not find requested sound." << "\n";
	return nullptr;
}

Weapon* AssetLoader::getWeapon(const std::string name) {
	auto find = weapons.find(name);

	if (find != weapons.end())
		return &(find->second);
	else
		std::cout << "AssetLoader error: Could not find requested weapon." << "\n";
	return nullptr;
}