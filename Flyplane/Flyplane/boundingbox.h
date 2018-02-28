#pragma once
#include "transform.h"
class Heightmap;
struct BoundingBox {
	glm::vec3 sides[3];
	glm::vec3 center;

	// only valid after setTransform is called
	glm::vec3 corners[8];
	glm::vec3 normals[3];
	glm::vec3 worldCenter;
	float boundingRadius = 0;

	bool intersect(BoundingBox &box);
	bool intersect(const glm::vec3& point);
	bool intersect(Heightmap *map);
	void setTransform(const Transform& transform);
};