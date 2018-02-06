#pragma once

#include <entityx\entityx.h>
#include "enginestate.h"

class EndlessMode : public EngineState {
private:
	entityx::EntityX ex;

	float points = 0;
	Timer deltatime, t;
	
public:
	void addPoints(int p) {
		points += p;
	}

	virtual ~EndlessMode() {}

	virtual void init();

	virtual void update(double dt);
};