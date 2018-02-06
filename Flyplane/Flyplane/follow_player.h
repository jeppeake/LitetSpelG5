#pragma once
#include "behaviour.h"
class Follow_Player : public Behaviour {
public:
	Follow_Player(int priority, Condition* condition) : Behaviour(priority, condition) {
	}
	glm::vec3 act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain) {
		std::cout << "DISTANCE TO TARGET: " << glm::length(player.component<Transform>()->pos - AI.component<Transform>()->pos) << "\n";
		glm::vec3 input = SAIB::flyTo(AI.component<Transform>()->pos, AI.component<Transform>()->orientation, player.component<Transform>()->pos);
		return input;
	}
};