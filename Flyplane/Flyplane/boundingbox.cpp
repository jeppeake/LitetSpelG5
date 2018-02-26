#include <glm/glm.hpp>
#include "boundingbox.h"
#include "heightmap.h"
#include <iostream>

void SATtest(const glm::vec3 &axis, const glm::vec3 *corners, float& minAlong, float& maxAlong)
{
	minAlong = std::numeric_limits<float>::max(), maxAlong = -std::numeric_limits<float>::max();
	for (int i = 0; i < 8; i++)
	{
		float dotval = glm::dot(corners[i], axis);
		if (dotval < minAlong) minAlong = dotval;
		if (dotval > maxAlong) maxAlong = dotval;
	}
}


bool isBetweenOrdered(float val, float lowerBound, float upperBound) {
	return lowerBound <= val && val <= upperBound;
}


bool overlaps(float min1, float max1, float min2, float max2)
{
	return isBetweenOrdered(min2, min1, max1) || isBetweenOrdered(min1, min2, max2);
}


bool BoundingBox::intersect(BoundingBox & box)
{
	//std::cout << "Start OBB-OBB intersection test\n";

	for (int i = 0; i < 3; i++)
	{
		float shape1Min, shape1Max, shape2Min, shape2Max;
		SATtest(normals[i], corners, shape1Min, shape1Max);
		SATtest(normals[i], box.corners, shape2Min, shape2Max);
		if (!overlaps(shape1Min, shape1Max, shape2Min, shape2Max))
		{
			return false;
		}
	}


	for (int i = 0; i < 3; i++)
	{
		float shape1Min, shape1Max, shape2Min, shape2Max;
		SATtest(box.normals[i], corners, shape1Min, shape1Max);
		SATtest(box.normals[i], box.corners, shape2Min, shape2Max);
		if (!overlaps(shape1Min, shape1Max, shape2Min, shape2Max))
		{
			return false;
		}
	}
	return true;
}


bool BoundingBox::intersect(const glm::vec3 & point)
{

	for (int i = 0; i < 3; i++)
	{
		float min, max;
		SATtest(normals[i], corners, min, max);
		float pdot = dot(normals[i], point);
		if (pdot <= min && pdot >= max)
		{
			return false;
		}
	}
	return true;
}

bool BoundingBox::intersect(Heightmap *map)
{
	for (int index = 0; index < 8; index++)
	{
		float height = map->heightAt(corners[index]);
		if (corners[index].y < height) {
			return true;
		}
	}
	return false;
}

void BoundingBox::setTransform(const Transform & transform)
{
	this->transform = transform;

	glm::vec3 rotSides[3];
	for (int i = 0; i < 3; i++) // maybe add scale in calculation
		rotSides[i] = transform.orientation*this->sides[i];

	normals[0] = glm::normalize(rotSides[0]);
	normals[1] = glm::normalize(rotSides[1]);
	normals[2] = glm::normalize(rotSides[2]);

	int i = 0;
	for (float x = -1; x <= 1; x += 2) {
		for (float y = -1; y <= 1; y += 2) {
			for (float z = -1; z <= 1; z += 2) {
				corners[i] = x * rotSides[0] + y * rotSides[1] + z * rotSides[2] + transform.orientation*center + transform.pos;
				i++;
			}
		}
	}
}
