#pragma once
#include "behaviour.h"
class Avoid_Closest : public Behaviour {
public:
	Avoid_Closest(int priority, Condition* condition) : Behaviour(priority, condition) {
	}
	Commands act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain, entityx::Entity closest) {
		Commands com;
		glm::vec3 cca2 = closest.component<Transform>()->pos + ((AI.component<Transform>()->pos - closest.component<Transform>()->pos) * 2.f);
		com.steering = SAIB::flyTo(AI.component<Transform>()->pos, AI.component<Transform>()->orientation, cca2);
		return com;
	}
};