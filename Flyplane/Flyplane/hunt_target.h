#pragma once
#include "behaviour.h"
class Hunt_Target : public Behaviour {
public:
	Hunt_Target(int priority, Condition* condition, entityx::Entity target, float cone, float distance) : Behaviour(priority, condition) {
		this->target = target;
		this->cone = cone;
		this->distance = distance;
	}
	Commands act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain, entityx::Entity closest, entityx::TimeDelta dt) {
		Commands com;
		if (target.valid()) {
			glm::vec3 aimPos;
			bool turretWeapon = false;
			int  turrPos = 0;
			if (AI.component<Equipment>()->primary.size() != 0) {
				aimPos = SAIB::ADVInterdiction(target, AI, AI.component<Equipment>()->primary.at(0).stats.speed, glm::vec3(), dt);
			}
			else if (AI.component<Equipment>()->turrets.size() != 0) {
				for (int i = 0; i < AI.component<Equipment>()->turrets.size(); i++) {
					if (!AI.component<Equipment>()->turrets.at(i).autoFire) {
						aimPos = SAIB::ADVInterdiction(target, AI, AI.component<Equipment>()->turrets.at(i).stats.speed, glm::vec3(), dt);
						turretWeapon = true;
						turrPos = i;
						break;
					}
				}
			}
			else {
				std::cout << "[Warning] Cannot calculate interdiction: no weapon on aircraft!\n";
				return com;
			}
			glm::vec3 AI_vector = glm::normalize(glm::toMat3(AI.component<Transform>()->orientation) * glm::vec3(0.0, 0.0, 1.0));
			glm::vec3 AI_position = AI.component<Transform>()->pos;
			glm::vec3 aimVector = aimPos - AI_position;

			if (!turretWeapon && glm::length(AI_vector - glm::normalize(aimVector)) < cone && glm::length(aimVector) < distance) {
				com.fire_primary = true;
			}
			else if (turretWeapon && glm::length((AI.component<Transform>()->orientation * AI.component<Equipment>()->turrets.at(turrPos).getGunOrientation() * glm::vec3(0.f, 0.f, 1.f)) - glm::normalize(aimVector)) < cone && glm::length(aimVector) < AI.component<Equipment>()->turrets.at(turrPos).info.range) {
				com.fire_primary = true;
			}
			if (AI.component<Equipment>()->special.size() > 0) {
				WeaponStats stats = AI.component<Equipment>()->special[AI.component<Equipment>()->selected].stats;
				if (stats.speed * stats.lifetime * 0.75f > glm::length(aimVector) && glm::length(AI_vector - glm::normalize(aimVector)) < cone) {
					com.fire_secondary = true;
				}
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