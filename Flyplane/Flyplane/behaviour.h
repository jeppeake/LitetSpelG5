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
	}
	~Behaviour() {
		delete condition;
	}
	virtual int getPriority() {
		return priority;
	}

	virtual glm::vec3 act(ComponentHandle<Transform> p_transform, ComponentHandle<FlightComponent> p_flight, ComponentHandle<Transform> me_transform, ComponentHandle<FlightComponent> me_flight, bool is_targeted) = 0; //add player, friendlies, (hostile) missiles and aircraft orientation to function call

	int priority = 0;
	bool active = false;
	Condition* condition;
};