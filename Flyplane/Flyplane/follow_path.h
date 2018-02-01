#pragma once
#include "behaviour.h"
class Follow_Path : public Behaviour {
public:
	Follow_Path(int priority, std::vector<glm::vec3> plotter, bool loop_mode) : Behaviour(priority) {
		this->plotter = plotter;
		this->loop_mode = loop_mode;
	}
	glm::vec3 act(ComponentHandle<Transform> p_transform, ComponentHandle<FlightComponent> p_flight, ComponentHandle<Transform> me_transform, ComponentHandle<FlightComponent> me_flight, bool is_targeted) {
		if (glm::length(me_transform->pos - plotter.at(target)) < 100) {
			target++;
			//std::cout << "Target " << target << " reached.\n";
			if (target == plotter.size()) {
				target = 0;
			}
		}
		//glm::vec3 input = SAIB::fly_to(me_transform->pos, me_transform->orientation, plotter.at(target));
		glm::vec3 input = SAIB::fly_to(me_transform->pos, me_transform->orientation, p_transform->pos);
		return input;
	}
	std::vector<glm::vec3> plotter;
	bool loop_mode;
	int target = 0;
};