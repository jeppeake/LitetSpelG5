#pragma once

#define FACTION_PLAYER 0
#define FACTION_AI 1
#define FACTION_DUMMY 2

struct Target {
	Target() {};
	Target(double heat, unsigned int faction) : heat(heat), faction(faction) {}
	double heat = 10;
	unsigned int faction = 1;
	bool is_targeted = false;
	bool preview_target = false;
};