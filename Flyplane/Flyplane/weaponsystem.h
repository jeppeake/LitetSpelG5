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
		ComponentHandle<Weapon> weapon;
		ComponentHandle<Transform> trans;

		for (Entity entity : es.entities_with_components(weapon)) {
			weapon = entity.component<Weapon>();
			if (weapon->playerOwned && GetAsyncKeyState(VK_F1) && ) {
				weapon->shouldFire = true;
			}
			if (weapon->shouldFire) {
				std::cout << "Shot gun at speed " << weapon->stats->speed << "\n";
				//spawn bullet/missile/bomb at trans, 
				weapon->shouldFire = false;
			}
		}
	};
};