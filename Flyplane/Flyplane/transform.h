#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>


struct Transform {
	Transform(glm::vec3 pos, glm::quat orientation = glm::quat()) : pos(pos), orientation(orientation) {}
	Transform(glm::vec3 pos, glm::quat orientation, glm::vec3 scale) : pos(pos), orientation(orientation), scale(scale) {}
	Transform() {}
	glm::vec3 pos;
	glm::quat orientation;
	glm::vec3 scale = glm::vec3(1);
};