#pragma once

#include "enginestate.h"
#include "buttonhandler.h"
#include "menustate.h"
#include "highscore.h"

class PreorderState : public EngineState {
private:
	string name;
	void checkForInput();
public:
	virtual ~PreorderState() {}
	virtual void init();
	virtual void update(double dt);
	void startMenu();
};