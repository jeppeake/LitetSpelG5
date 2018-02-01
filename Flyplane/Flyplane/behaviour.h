#pragma once

#include "transform.h"
#include "flightcomponent.h"
#include "SAIB.h"

class Behaviour {
public:
	Behaviour(int priority) {
		this->priority = priority;
	}
	virtual int getPriority() {
		return priority;
	}

	virtual glm::vec3 act(ComponentHandle<Transform> p_transform, ComponentHandle<FlightComponent> p_flight, ComponentHandle<Transform> me_transform, ComponentHandle<FlightComponent> me_flight, bool is_targeted) = 0; //add player, friendlies, (hostile) missiles and aircraft orientation to function call

	int priority = 0;
};