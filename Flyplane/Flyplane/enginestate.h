#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "timer.h"
#include "assetloader.h"

class EngineState {

	EngineState* new_state = nullptr;
	
protected:
	void changeState(EngineState* _new_state) {
		this->new_state = _new_state;
	}
public:

	EngineState * newState() {
		EngineState* temp = new_state;
		new_state = nullptr;
		return temp;
	}

	virtual ~EngineState() {}

	virtual void init() = 0;

	virtual void update(double dt) = 0;
};