#pragma once

#include "enginestate.h"
#include "buttonhandler.h"
#include "menustate.h"

class LoadingState : public EngineState {
private:
	ButtonHandler bHandler;
	bool done = false;
	std::string current;
	sf::Sound music;
	string name;
public:
	LoadingState(string name) : name(name) {}
	virtual ~LoadingState() {}
	virtual void init();
	virtual void update(double dt);
	void startGame();
};