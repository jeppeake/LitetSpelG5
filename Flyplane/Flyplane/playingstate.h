#pragma once 

#include <entityx\entityx.h>
#include "enginestate.h"


class PlayingState : public EngineState {
	Timer deltatime, t;
	entityx::EntityX ex;
	AssetLoader assetLoader;

public:

	virtual ~PlayingState() {}

	virtual void init();

	virtual void update(double dt);

};