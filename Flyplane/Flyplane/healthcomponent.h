#pragma once

struct HealthComponent {
	HealthComponent(double health) : health(health), maxHP(health) { }
	double health;
	double maxHP;
};