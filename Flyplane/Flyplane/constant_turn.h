#pragma once
#include "behaviour.h"
class Constant_Turn : public Behaviour {
public:
	Constant_Turn(int priority, Condition* condition) : Behaviour(priority, condition) {

	}
	glm::vec3 act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain) {
		return glm::vec3(0.f, 0.f, 1.f);
	}
};