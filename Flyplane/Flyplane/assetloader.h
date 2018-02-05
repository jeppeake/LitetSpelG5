#pragma once
#include <iostream>
#include <unordered_map>
#include "model.h"
#include "soundsystem.h"
#include "heightmap.h"

class AssetLoader {
private:
	std::unordered_map<std::string, Model> models;
	std::unordered_map<std::string, Heightmap> heightmaps;
	std::unordered_map<std::string, sf::SoundBuffer> sounds;
public:
	AssetLoader() {}
	~AssetLoader() {}
	void loadModel(const std::string filename, const std::string name);
	void loadHeightmap(const std::string map, const std::string tex, std::string name);
	void loadSound(const std::string filename, const std::string name);
	Model* getModel(const std::string filename);
	Heightmap* getHeightmap(const std::string name);
	sf::SoundBuffer* getSoundBuffer(const std::string filename);
};