#pragma once
#include "behaviour.h"
class Follow_Player : public Behaviour {
public:
	Follow_Player(int priority, Condition* condition) : Behaviour(priority, condition) {
	}
	Commands act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain, entityx::Entity closest, entityx::TimeDelta dt) {
		//std::cout << "DISTANCE TO TARGET: " << glm::length(player.component<Transform>()->pos - AI.component<Transform>()->pos) << "\n";
		Commands com;
		com.steering = SAIB::flyTo(AI.component<Transform>()->pos, AI.component<Transform>()->orientation, player.component<Transform>()->pos);
		return com;
	}
};