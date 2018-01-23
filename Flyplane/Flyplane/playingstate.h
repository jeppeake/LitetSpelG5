#pragma once 

#include <entityx\entityx.h>
#include "enginestate.h"


class PlayingState : public EngineState {
	Timer deltatime, t;


	entityx::EntityX ex;

public:

	virtual ~PlayingState() {}

	virtual void init();

	virtual void update();

};