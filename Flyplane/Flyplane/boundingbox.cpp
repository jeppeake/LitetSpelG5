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
	glm::vec3 sides[3];
	for (int i = 0; i < 3; i++) // maybe add scale in calculation
		sides[i] = glm::toMat3(transform.orientation)*this->sides[i];

	glm::vec3 normals[3];
	glm::vec3 corners[8];
	normals[0] = glm::normalize(sides[0]);
	normals[1] = glm::normalize(sides[1]);
	normals[2] = glm::normalize(sides[2]);
	int i = 0;
	for (float x = -1; x <= 1; x += 2)
	{
		for (float y = -1; y <= 1; y += 2)
		{
			for (float z = -1; z <= 1; z += 2)
			{
				corners[i] = x * sides[0] + y * sides[1] + z * sides[2] + glm::toMat3(transform.orientation)*center + transform.pos;
				i++;
			}
		}
	}

	glm::vec3 bsides[3];
	for (int i = 0; i < 3; i++)
		bsides[i] = glm::toMat3(box.transform.orientation)*box.sides[i];
		
	glm::vec3 bnormals[3];
	glm::vec3 bcorners[8];
	bnormals[0] = glm::normalize(bsides[0]);
	bnormals[1] = glm::normalize(bsides[1]);
	bnormals[2] = glm::normalize(bsides[2]);
	i = 0;
	for (float x = -1; x <= 1; x += 2)
	{
		for (float y = -1; y <= 1; y += 2)
		{
			for (float z = -1; z <= 1; z += 2)
			{
				bcorners[i] = x * bsides[0] + y * bsides[1] + z * bsides[2] + glm::toMat3(box.transform.orientation)*box.center + box.transform.pos;
				i++;
			}
		}
	}
	for (int i = 0; i < 3; i++)
	{
		float shape1Min, shape1Max, shape2Min, shape2Max;
		SATtest(normals[i], corners, shape1Min, shape1Max);
		SATtest(normals[i], bcorners, shape2Min, shape2Max);
		if (!overlaps(shape1Min, shape1Max, shape2Min, shape2Max))
		{
			return false;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		float shape1Min, shape1Max, shape2Min, shape2Max;
		SATtest(bnormals[i], corners, shape1Min, shape1Max);
		SATtest(bnormals[i], bcorners, shape2Min, shape2Max);
		if (!overlaps(shape1Min, shape1Max, shape2Min, shape2Max))
		{
			return false;
		}
	}
	return true;
}


bool BoundingBox::intersect(const glm::vec3 & point)
{
	glm::vec3 sides[3];
	for (int i = 0; i < 3; i++)
		sides[i] = glm::toMat3(transform.orientation)*this->sides[i];

	glm::vec3 normals[3];
	glm::vec3 corners[8];
	normals[0] = glm::normalize(sides[0]);
	normals[1] = glm::normalize(sides[1]);
	normals[2] = glm::normalize(sides[2]);
	int i = 0;
	for (float x = -1; x <= 1; x += 2)
	{
		for (float y = -1; y <= 1; y += 2)
		{
			for (float z = -1; z <= 1; z += 2)
			{
				corners[i] = x * sides[0] + y * sides[1] + z * sides[2] + glm::toMat3(transform.orientation)*center + transform.pos;
				i++;
			}
		}
	}
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
	glm::vec3 sides[3];
	for (int i = 0; i < 3; i++)
		sides[i] = glm::toMat3(transform.orientation)*this->sides[i];
	
	glm::vec3 normals[3];
	glm::vec3 corners[8];
	normals[0] = glm::normalize(sides[0]);
	normals[1] = glm::normalize(sides[1]);
	normals[2] = glm::normalize(sides[2]);
	int i = 0;
	for (float x = -1; x <= 1; x += 2)
	{
		for (float y = -1; y <= 1; y += 2)
		{
			for (float z = -1; z <= 1; z += 2)
			{
				corners[i] = x * sides[0] + y * sides[1] + z * sides[2] + glm::toMat3(transform.orientation)*center + transform.pos;
				i++;
			}
		}
	}
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
}
