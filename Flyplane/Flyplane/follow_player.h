#pragma once
#include "behaviour.h"
class Follow_Player : public Behaviour {
public:
	Follow_Player(int priority, Condition* condition) : Behaviour(priority, condition) {
	}
	glm::vec3 act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain) {
		//std::cout << "DISTANCE TO TARGET: " << glm::length(p_transform->pos - me_transform->pos) << "\n";
		glm::vec3 input = SAIB::fly_to(AI.component<Transform>()->pos, AI.component<Transform>()->orientation, player.component<Transform>()->pos);
		return input;
	}
};