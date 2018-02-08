#pragma once
#include <glm\glm.hpp>
#include <vector>
struct FormationComponent {
	FormationComponent(float distance){
		this->distance = distance;
	}
	std::vector<glm::vec2> positions;
	bool add = true;
	float mult = 1.f;
	int posCalc = 0;
	float distance;
};