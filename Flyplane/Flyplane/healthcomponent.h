#pragma once

struct HealthComponent {
	HealthComponent(double health) : health(health), maxHP(health) { }
	bool isDead = false;
	double health;
	double maxHP;
};