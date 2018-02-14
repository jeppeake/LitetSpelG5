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
		transform.push_back(glm::translate(glm::vec3(0, 10000, 0)) * ) * glm::scale(glm::vec3(500, 300, 1)));
	}
};