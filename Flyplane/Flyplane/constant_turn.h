#pragma once
#include "behaviour.h"
class Constant_Turn : public Behaviour {
public:
	Constant_Turn(int priority, Condition* condition) : Behaviour(priority, condition) {

	}
	Commands act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain, entityx::Entity closest, entityx::TimeDelta dt) {
		Commands com;
		com.steering = glm::vec3(0.f, 0.f, 1.f);
		return com;
	}
};