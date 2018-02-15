#pragma once

#include "texture.h"
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

struct Cloud {
	Texture texture;
	std::vector<glm::mat4> transform;
	Cloud() {
		texture.loadTexture("assets/textures/cloud.png", 1);

		for (int i = 0; i < 3; i++) {
			for (int k = 0; k < 4; k++) {
				transform.push_back(glm::translate(glm::vec3(50000 * i + rand() % 40000, 40000, 50000 * k + rand() % 40000)) * glm::rotate(glm::radians<float>(rand() % 360), glm::vec3(0, 1, 0)) * glm::rotate(3.14f / 2.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(10000, 6000, 1)));
			}
		}
	}
};