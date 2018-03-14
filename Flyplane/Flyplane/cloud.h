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

		int xCount= 5;
		int zCount = 5;
		for (int i = 0; i < xCount; i++) {
			for (int k = 0; k < zCount; k++) {
				glm::vec3 pos;
				pos.x = 50000 * i + rand() % 40000 - xCount *50000/2;
				pos.y = 40000;
				pos.z = 50000 * k + rand() % 40000 - zCount *50000/2;
				transform.push_back(glm::translate(pos) * glm::rotate(glm::radians<float>(rand() % 360), glm::vec3(0, 1, 0)) * glm::rotate(3.14f / 2.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(10000, 6000, 1)));
			}
		}
	}
};