#pragma once
#include "behaviour.h"
class Constant_Turn : public Behaviour {
public:
	Constant_Turn(int priority) : Behaviour(priority) {

	}
	glm::vec3 act(ComponentHandle<Transform> p_transform, ComponentHandle<FlightComponent>, ComponentHandle<Transform> me_transform, ComponentHandle<FlightComponent> me_flight, bool is_targeted) {
		return glm::vec3(0.f, 0.f, 1.f);
	}
};