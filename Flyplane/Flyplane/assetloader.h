#pragma once
#include <iostream>
#include <unordered_map>
#include "model.h"
#include "soundcomponent.h"
#include "heightmap.h"
#include "text.hpp"
#include "texture.h"

class AssetLoader {
private:
	std::unordered_map<std::string, Model> models;
	std::unordered_map<std::string, Texture> textures;
	std::unordered_map<std::string, Heightmap> heightmaps;
	std::unordered_map<std::string, sf::SoundBuffer> sounds;
	Text *text = nullptr;
	Text *menutext = nullptr;
	Text *bigtext = nullptr;
	Text *highscoreText = nullptr;
public:
	AssetLoader() {
		text = new Text("assets/fonts/arial.ttf", 48);
		menutext = new Text("assets/fonts/war2.ttf", 36);
		bigtext = new Text("assets/fonts/war2.ttf", 48);
		highscoreText = new Text("assets/fonts/consola.ttf", 36);
	}
	~AssetLoader() {}
	static AssetLoader &getLoader()
	{
		static AssetLoader s;
		return s;
	}
	Text *getText() &;
	Text *getMenutext() &;
	Text *getBigtext() &;
	Text *getHighscoreText() &;
	AssetLoader(const AssetLoader &other) = delete;
	void operator=(const AssetLoader &other) = delete;
	void loadModel(const std::string &filename, const std::string &name);
	void loadHeightmap(const std::string &maptxt, const std::string &name);
	void loadSound(const std::string &filename, const std::string &name);
	void loadTexture(const std::string filename, const std::string name);
	void clearTerrains() {
		this->heightmaps.clear();
	}
	Model* getModel(const std::string name);
	Texture* getTexture(const std::string name);
	Heightmap* getHeightmap(const std::string name);
	sf::SoundBuffer* getSoundBuffer(const std::string filename);
};