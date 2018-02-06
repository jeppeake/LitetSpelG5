#pragma once

#include "enginestate.h"
#include "highscore.h"

class GameOverState : public EngineState {
private:
	Highscore highscore;
public:
	GameOverState(string name, int score) {
		highscore.addScore(name, score);
	}
	virtual ~GameOverState();
	virtual void init();
	virtual void update(double dt);
};