#pragma once

#include "boundingbox.h"

struct CollisionComponent
{
	std::vector<BoundingBox> boxes;
};