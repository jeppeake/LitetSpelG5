#pragma once
#include "behaviour.h"
class Fly_To : public Behaviour {
public:
	Fly_To(int priority, std::vector<glm::vec3> plotter, bool loop_mode) : Behaviour(priority) {
		this->plotter = plotter;
		this->loop_mode = loop_mode;
	}
	glm::vec3 act(ComponentHandle<Transform> p_transform, ComponentHandle<FlightComponent>, ComponentHandle<Transform> me_transform, ComponentHandle<FlightComponent> me_flight, bool is_targeted) {
		/*for (int i = 0; i < plotter.size(); i++) {
			std::cout << plotter.at(i).x << " : " << plotter.at(i).y << " : " << plotter.at(i).z << "\n";
		}*/

		glm::vec3 front = glm::toMat3(me_transform->orientation) * glm::vec3(0.0, 0.0, 1.0);
		glm::vec3 up = glm::toMat3(me_transform->orientation) * glm::vec3(0.0, 1.0, 0.0);
		glm::vec3 left = glm::toMat3(me_transform->orientation) * glm::vec3(1.0, 0.0, 0.0);

		glm::vec3 t = plotter.at(target);

		glm::vec3 pt = t - me_transform->pos;
		//std::cout << "Dist to waypoint: " << glm::length(pt) << " dist to player: " << glm::length(me_transform->pos - p_transform->pos) << "\n";
		if (glm::length(pt) < 100) {
			target++;
			std::cout << "Target " << target << " reached.\n";
			if (target == plotter.size()) {
				target = 0;
			}
		}
		pt = glm::normalize(pt);

		glm::vec3 pt_proj_on_front = glm::proj(pt, front);

		glm::vec3 pt_adjusted_for_roll = glm::normalize(pt - pt_proj_on_front);
		//std::cout << glm::length(up - pt_adjusted_for_roll) << std::endl;
		
		if (glm::length(up - pt_adjusted_for_roll) < 0.2) {//engage pitch mode
			//std::cout << "Pitch mode\n";
			roll *= -1;
			glm::vec3 input = glm::vec3(0.0, 0.0, 0.0);
			if (glm::length(front - pt) < 0.2) {
				input += glm::vec3(0.0, 0.0, 0.0);//fly to target
			}
			else {
				input += glm::vec3(0.0, 1.0, 0.0);//pitch towards target
			}
			return input;
		}
		else {//continue roll
			//std::cout << "Roll mode\n";
			return glm::vec3(roll, 0.0, 0.0);
		}

		return glm::vec3(0.f, 0.f, 0.f);
	}
	std::vector<glm::vec3> plotter;
	bool loop_mode;
	int target = 0;
	float roll = 1.0;
};