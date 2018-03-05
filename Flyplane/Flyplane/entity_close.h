#pragma once
#include "condition.h"
class Entity_Close : public Condition {
public:
	Entity_Close(float distance) : Condition() {
		this->distance = distance;
	}
	bool test(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain, entityx::Entity closest) {
		//std::cout << "Testing\n";
		if (closest.valid()) {
			return (((glm::dot(glm::normalize(closest.component<Transform>()->pos - AI.component<Transform>()->pos), glm::normalize(AI.component<Physics>()->velocity)) + 1.0f) / 2.f) * glm::length(closest.component<Transform>()->pos - AI.component<Transform>()->pos) < distance);
		}
		else {
			return false;
		}
	}
	float distance;
};