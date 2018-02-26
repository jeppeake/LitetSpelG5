#pragma once
#include "behaviour.h"
class Hunt_Player : public Behaviour {
public:
	Hunt_Player(int priority, Condition* condition, float cone, float distance) : Behaviour(priority, condition) {
		this->cone = cone;
		this->distance = distance;
	}
	Commands act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain, entityx::Entity closest, entityx::TimeDelta dt) {
		Commands com;
		glm::vec3 aimPos = SAIB::calculateInterdiction(player, AI);
		glm::vec3 AI_vector = glm::normalize(glm::toMat3(AI.component<Transform>()->orientation) * glm::vec3(0.0, 0.0, 1.0));
		glm::vec3 AI_position = AI.component<Transform>()->pos;
		glm::vec3 aimVector = aimPos - AI_position;

		if (glm::length(AI_vector - glm::normalize(aimVector)) < cone && glm::length(aimVector) < distance) {
			com.fire_primary = true;
		}
		com.steering = SAIB::flyTo(AI.component<Transform>()->pos, AI.component<Transform>()->orientation, aimPos);
		return com;
	}
	float cone, distance;
};