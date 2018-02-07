#pragma once

#include "enginestate.h"
#include "buttonhandler.h"
#include "menustate.h"
#include "highscore.h"

class HighscoreState : public EngineState {
private:
	ButtonHandler bHandler;
	string* highscore;
public:
	HighscoreState() {
		highscore = Highscore::getHighscore().getHighscoreList();
	}
	virtual ~HighscoreState() {}
	virtual void init();
	virtual void update(double dt);
	void startMenu();
};