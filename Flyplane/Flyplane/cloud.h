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
		transform.push_back(glm::translate(glm::vec3(0, 10000, 0)) * glm::rotate(3.14f/2.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(5000, 3000, 1)));
		transform.push_back(glm::translate(glm::vec3(20000, 10000, 20000)) * glm::rotate(3.14f / 2.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(5000, 3000, 1)));
		transform.push_back(glm::translate(glm::vec3(-10000, 10000, 5000)) * glm::rotate(3.14f / 2.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(5000, 3000, 1)));
		transform.push_back(glm::translate(glm::vec3(-10000, 10000, -5000)) * glm::rotate(3.14f / 2.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(5000, 3000, 1)));
		transform.push_back(glm::translate(glm::vec3(10000, 10000, -3000)) * glm::rotate(3.14f / 2.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(5000, 3000, 1)));
		transform.push_back(glm::translate(glm::vec3(10000, 10000, -3000)) * glm::rotate(3.14f / 2.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(5000, 3000, 1)));
	}
};