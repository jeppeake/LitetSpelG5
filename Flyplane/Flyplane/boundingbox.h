#pragma once

#include "transform.h"

struct BoundingBox {
	glm::vec3 sides[3];
	glm::vec3 center;
	
	Transform transform;

	bool intersect(BoundingBox &box);
	bool intersect(const glm::vec3& point);

	void setTransform(const Transform& transform);
};