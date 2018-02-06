#pragma once 

#include <entityx\entityx.h>
#include "enginestate.h"
#include "buttonhandler.h"
#include "ButtonAction.h"



class PlayingState : public EngineState {
	Timer deltatime, t;
	entityx::EntityX ex;
	AssetLoader assetLoader;
private:
	ButtonHandler bHandler;
	bool menuOpen = false;

public:

	virtual ~PlayingState() {}

	virtual void init();

	virtual void update(double dt);

	void startMenu();

	void restart();

};