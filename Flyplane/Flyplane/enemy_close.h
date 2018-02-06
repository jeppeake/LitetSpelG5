#pragma once
#include "condition.h"
class Enemy_Close : public Condition {
public:
	Enemy_Close(float distance) : Condition() {
		this->distance = distance;
	}
	bool test(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain) {
		//std::cout << (glm::length(p_transform->pos - me_transform->pos) < distance) << "\n";
		return (glm::length(player.component<Transform>()->pos - AI.component<Transform>()->pos) < distance);
	}
	float distance;
};