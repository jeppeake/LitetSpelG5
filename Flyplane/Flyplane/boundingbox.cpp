#include <glm/glm.hpp>
#include "boundingbox.h"
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
				corners[i] = x * sides[0] + y * sides[1] + z * sides[2] + center;
				i++;
			}
		}
	}
	glm::vec3 bnormals[3];
	glm::vec3 bcorners[8];
	bnormals[0] = glm::normalize(box.sides[0]);
	bnormals[1] = glm::normalize(box.sides[1]);
	bnormals[2] = glm::normalize(box.sides[2]);
	i = 0;
	for (float x = -1; x <= 1; x += 2)
	{
		for (float y = -1; y <= 1; y += 2)
		{
			for (float z = -1; z <= 1; z += 2)
			{
				bcorners[i] = x * box.sides[0] + y * box.sides[1] + z * box.sides[2] + box.center;
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
				corners[i] = x * sides[0] + y * sides[1] + z * sides[2] + center;
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
