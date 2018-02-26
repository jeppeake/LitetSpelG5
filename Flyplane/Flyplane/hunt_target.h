#pragma once
#include "behaviour.h"
class Hunt_Target : public Behaviour {
public:
	Hunt_Target(int priority, Condition* condition, entityx::Entity target, float cone, float distance) : Behaviour(priority, condition) {
		this->target = target;
		this->cone = cone;
		this->distance = distance;
	}
	Commands act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain, entityx::Entity closest) {
		Commands com;
		if (target.valid()) {
			glm::vec3 aimPos = SAIB::calculateInterdiction(target, AI);
			glm::vec3 AI_vector = glm::normalize(glm::toMat3(AI.component<Transform>()->orientation) * glm::vec3(0.0, 0.0, 1.0));
			glm::vec3 AI_position = AI.component<Transform>()->pos;
			glm::vec3 aimVector = aimPos - AI_position;

			if (glm::length(AI_vector - glm::normalize(aimVector)) < cone && glm::length(aimVector) < distance) {
				com.fire_primary = true;
			}
			float speed = AI.component<FlightComponent>()->base_speed;
			closingSpeed = lastDist - glm::length(aimVector);
			lastDist = glm::length(aimVector);
			//std::cout << closingSpeed << "\n";
			if (glm::length(aimVector) > distance) {
				if (closingSpeed < speed / 8) {
					com.throttle = 1.f;
				}
				else if (closingSpeed > speed / 8) {
					com.brake = 1.f;
				}
			}
			else if (AI.component<FlightComponent>()->i_pitch > 0.5) {
				com.brake = AI.component<FlightComponent>()->i_pitch;
			}
			com.steering = SAIB::flyTo(AI.component<Transform>()->pos, AI.component<Transform>()->orientation, aimPos);
		}
		else {
			//std::cout << "Terminated\n";
			terminated = true;
		}
		return com;
	}
	entityx::Entity target;
	float cone, distance;
	float closingSpeed = 0;
	float lastDist = 0;
};