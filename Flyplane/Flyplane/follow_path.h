#pragma once
#include "behaviour.h"
class Follow_Path : public Behaviour {
public:
	Follow_Path(int priority, Condition* condition, std::vector<glm::vec3> plotter, bool loop_mode) : Behaviour(priority, condition) {
		this->plotter = plotter;
		this->loop_mode = loop_mode;
	}
	Commands act(entityx::Entity player, entityx::Entity AI, entityx::Entity terrain) {
		if (glm::length(AI.component<Transform>()->pos - plotter.at(target)) < 100) {
			target++;
			//std::cout << "Target " << target << " reached.\n";
			if (target == plotter.size()) {
				target = 0;
			}
		}
		Commands com;
		com.steering = SAIB::flyTo(AI.component<Transform>()->pos, AI.component<Transform>()->orientation, plotter.at(target));
		//glm::vec3 input = SAIB::fly_to(me_transform->pos, me_transform->orientation, p_transform->pos);
		return com;
	}
	std::vector<glm::vec3> plotter;
	bool loop_mode;
	int target = 0;
};