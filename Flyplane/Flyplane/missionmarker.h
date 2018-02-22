#pragma once
#include "glm\glm.hpp"

#define MARKER_KILL glm::vec3(1,1,0)
#define MARKER_DEFEND glm::vec3(1,0,1)

struct MissionMarker {
	MissionMarker(glm::vec3 color) : color(color) {}
	glm::vec3 color;
};