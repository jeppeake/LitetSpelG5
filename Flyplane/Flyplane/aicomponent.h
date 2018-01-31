#pragma once
#include "behaviour.h"
struct AIComponent {
	AIComponent(){}

	std::vector<Behaviour> behaviours;
};