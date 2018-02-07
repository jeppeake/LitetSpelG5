#pragma once

#include "enginestate.h"
#include "buttonhandler.h"
#include "menustate.h"

class LoadoutState : public EngineState {
private:
	ButtonHandler bHandler;
public:
	virtual ~LoadoutState() {}
	virtual void init();
	virtual void update(double dt);
	void startMenu();
};