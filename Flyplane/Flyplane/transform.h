#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>


struct Transform {
	Transform(glm::vec3 pos, glm::quat orientation) : pos(pos), orientation(orientation) {}
	Transform() {}
	glm::vec3 pos;
	glm::quat orientation;
	glm::vec3 scale;
};