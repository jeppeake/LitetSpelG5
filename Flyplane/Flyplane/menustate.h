#pragma once

#include "enginestate.h"
#include "buttonhandler.h"
#include "optionsstate.h"

class MenuState : public EngineState {
private:
	entityx::EntityX ex;
	ButtonHandler bHandler;
	void spawnEnemies(int nr);
public:
	virtual ~MenuState();
	virtual void init();
	virtual void update(double dt);
	void startGame();
	void startOptions();
	void startLoadout();
	void startHighscore();
};