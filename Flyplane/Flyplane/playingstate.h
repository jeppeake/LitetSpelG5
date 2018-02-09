#pragma once 

#include <entityx\entityx.h>
#include "enginestate.h"
#include "buttonhandler.h"
#include "ButtonAction.h"
#include "highscore.h"
#include "map.h"

class PlayingState : public EngineState {
	Timer deltatime, t;
	entityx::EntityX ex;
	AssetLoader assetLoader;
private:

	ButtonHandler bHandler;
	bool menuOpen = false;
	bool playerAlive = true;
	float points = 0;

	sf::SoundBuffer* flyingSB;
	sf::SoundBuffer* bulletSB;
	sf::SoundBuffer* machinegunSB;
	sf::SoundBuffer* machinegunShortSB;
	sf::SoundBuffer* missileSB;

	void spawnEnemies(int nr);
	void drawHighscore();

	std::string name;

	void loadLoadout();

public:
	PlayingState(std::string name = "name") : name(name) {}
	virtual ~PlayingState() {}

	virtual void init();

	virtual void update(double dt);

	void startMenu();

	void restart();

	void gameOver();

	void addPoints(int p) {
		points += p;
	}

};