#pragma once

#include "enginestate.h"
#include "buttonhandler.h"
#include "menustate.h"

class OptionsState : public EngineState {
private:
	ButtonHandler bHandler;
public:
	virtual ~OptionsState() {}
	virtual void init();
	virtual void update(double dt);
	void startMenu();
};