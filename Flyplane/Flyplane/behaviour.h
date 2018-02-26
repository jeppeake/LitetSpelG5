#pragma once

#include "transform.h"
#include "flightcomponent.h"
#include "FormationComponent.h"
#include "SAIB.h"
#include <entityx\entityx.h>
#include <entityx\Entity.h>

#include "condition.h"

using namespace entityx;
struct Commands {
	bool fire_secondary = false;
	bool fire_primary = false;
	glm::vec3 steering = glm::vec3(0.0, 0.0, 0.0);
	float throttle = 0.f;
	float brake = 0.f;
};
class Behaviour {
public:

	Behaviour(int priority, Condition* condition) {
		this->priority = priority;
		this->condition = condition;
	}
	~Behaviour() {
		std::cout << "Deleting condition\n";
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

	virtual Commands act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain, entityx::Entity closest, entityx::TimeDelta dt) = 0; //add player, friendlies, (hostile) missiles and aircraft orientation to function call
	int priority = 0;
	bool active = false;
	bool terminated = false;
	Condition* condition;
};