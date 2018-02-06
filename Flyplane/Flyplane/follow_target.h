#pragma once
#include "behaviour.h"
class Follow_Target : public Behaviour {
public:
	Follow_Target(int priority, Condition* condition, entityx::Entity target) : Behaviour(priority, condition) {
		this->target = target;
	}
	glm::vec3 act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain) {
		std::cout << "DISTANCE TO TARGET: " << glm::length(target.component<Transform>()->pos - AI.component<Transform>()->pos) << "\n";
		glm::vec3 input = SAIB::flyTo(AI.component<Transform>()->pos, AI.component<Transform>()->orientation, target.component<Transform>()->pos);
		return input;
	}
	entityx::Entity target;
};