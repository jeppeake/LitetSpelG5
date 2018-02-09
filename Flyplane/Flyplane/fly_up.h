#pragma once
#include "behaviour.h"
class Fly_Up : public Behaviour {
public:
	Fly_Up(int priority, Condition* condition) : Behaviour(priority, condition) {

	}
	Commands act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain, entityx::Entity closest) {
		//std::cout << "AVOIDING TERRAIN!\n";
		glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
		glm::vec3 pos = AI.component<Transform>()->pos;
		glm::vec3 to = pos + (up * 200.f);
		Commands com;
		com.steering = SAIB::fly_to(AI.component<Transform>()->pos, AI.component<Transform>()->orientation, to);
		return com;
	}
};