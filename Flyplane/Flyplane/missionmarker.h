#pragma once
#include "glm\glm.hpp"

#define MARKER_KILL glm::vec3(0.89,0.71,0.11)
#define MARKER_DEFEND glm::vec3(0.47,0.22,9.74)

struct MissionMarker {
	MissionMarker(glm::vec3 color) : color(color) {}
	glm::vec3 color;
};