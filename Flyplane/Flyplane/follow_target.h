#pragma once
#include "behaviour.h"
class Follow_Target : public Behaviour {
public:
	Follow_Target(int priority, Condition* condition, entityx::Entity target) : Behaviour(priority, condition) {
		this->target = target;
	}
	Commands act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain) {
		Commands com;
		if (target.valid()) {
			//std::cout << "DISTANCE TO TARGET: " << glm::length(target.component<Transform>()->pos - AI.component<Transform>()->pos) << "\n";
			com.steering = SAIB::flyTo(AI.component<Transform>()->pos, AI.component<Transform>()->orientation, target.component<Transform>()->pos);
		}
		else {
			//std::cout << "INVALID!" << std::endl;
			terminated = true;
		}
		return com;
	}
	entityx::Entity target;
};