#pragma once
#include "transform.h"
class Heightmap;
struct BoundingBox {
	glm::vec3 sides[3];
	glm::vec3 center;
	
	Transform transform;

	bool intersect(BoundingBox &box);
	bool intersect(const glm::vec3& point);
	bool intersect(Heightmap *map);
	void setTransform(const Transform& transform);

	glm::vec3 getWorldCenter() {
		return glm::toMat3(transform.orientation)*(transform.scale*center) + transform.pos;
	}
};