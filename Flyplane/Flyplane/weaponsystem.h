#pragma once
#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "transform.h"
#include "weapon.h"
#include <iostream>
#include <Windows.h>

using namespace entityx;

struct WeaponSystem : public entityx::System<WeaponSystem> {
	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {
		ComponentHandle<WeaponStats> stats;
		ComponentHandle<Equipment> equip;

		for (Entity entity : es.entities_with_components(equip)) {
			stats = entity.component<WeaponStats>();
			equip = entity.component<Equipment>();
			if (GetAsyncKeyState(VK_F1))
			{
				std::cout << "Shot gun at speed " << equip->slots[equip->selected].component<WeaponStats>()->speed << "\n";
			}

			if (GetAsyncKeyState(VK_F2))
			{
				equip->selected = (equip->selected + 1) % equip->slots.size();
				std::cout << "Switched to weapon: " << equip->selected << "\n";
			}
		}
	};
};