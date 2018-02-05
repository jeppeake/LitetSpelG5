#pragma once

#include "enginestate.h"


class MenuState : public EngineState {
public:
	virtual ~MenuState();
	virtual void init();
	virtual void update(double dt);
};