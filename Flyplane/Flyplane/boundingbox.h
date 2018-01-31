#pragma once
struct BoundingBox {
	glm::vec3 sides[3];
	glm::vec3 center;
	bool intersect(BoundingBox &box);
};