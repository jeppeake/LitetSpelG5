#pragma once 

#include <entityx\entityx.h>
#include "enginestate.h"
#include "map.h"

class PlayingState : public EngineState {
	Timer deltatime, t;


	entityx::EntityX ex;
	Map map;
public:

	virtual ~PlayingState() {}

	virtual void init();

	virtual void update(double dt);

};