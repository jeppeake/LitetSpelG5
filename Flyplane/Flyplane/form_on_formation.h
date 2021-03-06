#pragma once
#include "behaviour.h"
class Form_On_Formation : public Behaviour {
public:
	Form_On_Formation(int priority, Condition* condition, entityx::Entity formation) : Behaviour(priority, condition) {
		this->formation = formation;
	}
	Commands act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain, entityx::Entity closest, entityx::TimeDelta dt) {
		Commands com;
		if (formation.valid()) {
			if (!formation.has_component<FormationComponent>() || formation.component<HealthComponent>()->isDead) {
				std::cout << "Formation terminated\n";
				terminated = true;
			} else if (id == -1) {//add position on formation
				id = formation.component<FormationComponent>()->positions.size();
				float posCalc = formation.component<FormationComponent>()->posCalc;
				glm::vec2 position(posCalc * formation.component<FormationComponent>()->mult, -posCalc);
				formation.component<FormationComponent>()->mult *= -1.f;
				if (formation.component<FormationComponent>()->add) {
					posCalc++;
				}
				formation.component<FormationComponent>()->posCalc = posCalc;
				formation.component<FormationComponent>()->add = !formation.component<FormationComponent>()->add;
				formation.component<FormationComponent>()->positions.push_back(position);
				//std::cout << position.x << " : " << position.y << "\n";
			} else {
				glm::quat ori = formation.component<Transform>()->orientation;
				glm::vec2 pos = formation.component<FormationComponent>()->positions.at(id);
				glm::vec3 w_pos = ori * glm::vec3(pos.x, 0.f, pos.y);
				glm::vec3 flyToPos = formation.component<Transform>()->pos + (w_pos * formation.component<FormationComponent>()->distance);
				//std::cout << "Flying on formation" << "\n";
				com.steering = SAIB::flyTo(AI.component<Transform>()->pos, AI.component<Transform>()->orientation, flyToPos);
				
				glm::vec3 AI_front = glm::normalize(glm::toMat3(AI.component<Transform>()->orientation) * glm::vec3(0.0, 0.0, 1.0));
				glm::vec3 pt = flyToPos - AI.component<Transform>()->pos;
				if (glm::length(pt) > 5.f) {
					com.throttle = glm::pow(glm::clamp(length(pt), 0.f, 1000.f) / 1000.f, 2.f);
				}
				//std::cout << com.throttle << "\n";
				//std::cout << glm::length(player.component<Transform>()->pos - AI.component<Transform>()->pos) << "\n";
			}
			
		}
		else {
			//std::cout << "Formation terminated\n";
			terminated = true;
		}

		return com;
	}
	entityx::Entity formation;
	int id = -1;
};