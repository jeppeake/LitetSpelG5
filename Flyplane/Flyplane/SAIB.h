#pragma once
#include "glm/gtx/projection.hpp"
#include "glm/gtx/rotate_vector.hpp"
namespace SAIB {//STANDARD ARTIFICIAL INTELLIGENCE BEHAVIOURS

	glm::vec3 fly_to(glm::vec3 position, glm::quat orientation, glm::vec3 target) {//fly to target, ignore orientation
		glm::vec3 front = glm::toMat3(orientation) * glm::vec3(0.0, 0.0, 1.0);
		glm::vec3 up = glm::toMat3(orientation) * glm::vec3(0.0, 1.0, 0.0);
		glm::vec3 left = glm::toMat3(orientation) * glm::vec3(1.0, 0.0, 0.0);

		glm::vec3 t = target;

		glm::vec3 pt = t - position;
		//std::cout << "Dist to waypoint: " << glm::length(pt) << " dist to player: \n";
		pt = glm::normalize(pt);

		glm::vec3 pt_proj_on_front = glm::proj(pt, front);

		glm::vec3 pt_adjusted_for_roll = glm::normalize(pt - pt_proj_on_front);
		glm::vec3 right_t = glm::rotate(up, glm::radians(5.f), front);
		glm::vec3 left_t = glm::rotate(up, glm::radians(-5.f), front);
		float roll = 1.f;
		if (length(left_t - pt_adjusted_for_roll) < length(right_t - pt_adjusted_for_roll)) {
			roll = -1.f;
		}
		//std::cout << glm::length(up - pt_adjusted_for_roll) << std::endl;

		if (glm::length(up - pt_adjusted_for_roll) < 0.2) {//engage pitch mode
														   //std::cout << "Pitch mode\n";
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
}