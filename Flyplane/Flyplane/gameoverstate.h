#pragma once

#include "enginestate.h"

class GameOverState : public EngineState {
	virtual ~GameOverState();
	virtual void init();
	virtual void update(double dt);
};