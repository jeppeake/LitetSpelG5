#pragma once

#include "enginestate.h"
#include "buttonhandler.h"
#include "menustate.h"
#include "fullscreenimage.h"

class OptionsState : public EngineState {
private:
	ButtonHandler bHandler;
	FullscreenImage fi;
public:
	virtual ~OptionsState() {}
	virtual void init();
	virtual void update(double dt);
	void startMenu();
};