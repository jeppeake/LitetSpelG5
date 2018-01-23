#pragma once

struct Equipment {
	//projectile model, sound, range?, ammo, speed, 
	Equipment(std::vector<Entity> slots) : slots(slots) {}

	unsigned int selected = 0;
	std::vector<Entity> slots;
};