#pragma once
#include "heightmap.h"
struct Terrain {
	Terrain(Heightmap* hmptr) : hmptr(hmptr) {}
	Heightmap* hmptr;
};