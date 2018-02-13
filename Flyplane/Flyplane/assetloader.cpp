#include "assetloader.h"

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
		Model model;
		models[name] = model;
		models[name].load(filename);
	}
}

void AssetLoader::loadTexture(const std::string filename, const std::string name) {
	auto find = textures.find(name);
	if (find == textures.end()) {
		Texture texture;
		textures[name] = texture;
		bool success = textures[name].loadTexture(filename);
	}
}

void AssetLoader::loadHeightmap(const std::string &maptxt, const std::string &name) {
	auto find = heightmaps.find(name);
	if (find == heightmaps.end()) {
		Heightmap hmap;
		heightmaps[name].loadMap(maptxt);
	}
}

void AssetLoader::loadSound(const std::string &filename, const std::string &name) {
	auto find = sounds.find(name);
	if (find == sounds.end()) {
		sf::SoundBuffer sb;
		sounds[name] = sb;
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