#pragma once

#include "enginestate.h"
#include "buttonhandler.h"
#include "menustate.h"
#include "highscore.h"

class NameState : public EngineState {
private:
	string name;
	void checkForInput();
public:
	virtual ~NameState() {}
	virtual void init();
	virtual void update(double dt);
	void startMenu();
};