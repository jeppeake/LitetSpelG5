#pragma once

#include "transform.h"
#include "flightcomponent.h"
#include "SAIB.h"
#include <entityx\entityx.h>
#include <entityx\Entity.h>

#include "condition.h"

using namespace entityx;
class Behaviour {
public:
	Behaviour(int priority, Condition* condition) {
		this->priority = priority;
		this->condition = condition;
	}
	~Behaviour() {
		delete condition;
	}
	virtual int getPriority() {
		return priority;
	}
	void setActive(bool active) {
		this->active = active;
	}
	bool getActive() {
		return active;
	}

	virtual glm::vec3 act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain) = 0; //add player, friendlies, (hostile) missiles and aircraft orientation to function call

	int priority = 0;
	bool active = false;
	Condition* condition;
};