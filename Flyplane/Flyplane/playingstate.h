#pragma once 

#include <entityx\entityx.h>
#include "enginestate.h"


class PlayingState : public EngineState {
	Timer deltatime, t;
	entityx::EntityX ex;
	AssetLoader assetLoader;
	float points = 0;

	sf::SoundBuffer* flyingSB;
	sf::SoundBuffer* bulletSB;
	sf::SoundBuffer* machinegunSB;
	sf::SoundBuffer* missileSB;

	void spawnEnemies(int nr);

	std::string name;

public:
	PlayingState() {};
	PlayingState(std::string name) {
		name = name;
	};
	virtual ~PlayingState() {}

	virtual void init();

	virtual void update(double dt);

	void gameOver();

	void addPoints(int p) {
		points += p;
	}

};