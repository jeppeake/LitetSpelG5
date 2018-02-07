#pragma once

#include "enginestate.h"
#include "buttonhandler.h"
#include "menustate.h"

class HighscoreState : public EngineState {
private:
	ButtonHandler bHandler;
public:
	virtual ~HighscoreState() {}
	virtual void init();
	virtual void update(double dt);
	void startMenu();
};