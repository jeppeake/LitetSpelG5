#pragma once

#include "enginestate.h"
#include "buttonhandler.h"
#include "menustate.h"
#include "fullscreenimage.h"
#include <string>

class LoadingState : public EngineState {
private:
	FullscreenImage fi;
	ButtonHandler bHandler;
	bool done = false;
	std::string current;
	sf::Sound music;
	std::string name;
public:
	LoadingState(std::string name) : name(name) {}
	virtual ~LoadingState() {}
	virtual void init();
	virtual void update(double dt);
	void startGame();
};