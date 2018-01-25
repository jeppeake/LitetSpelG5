#pragma once
#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "transform.h"
#include "weapon.h"
#include <iostream>
#include <Windows.h>
#include "input.h"
#include "modelcomponent.h"
#include "equipment.h"
#include "playercomponent.h"
#include <ctime>

using namespace entityx;

struct WeaponSystem : public entityx::System<WeaponSystem> {
	unsigned int playerActiveWeapon = 0;
	unsigned int nrPlayerWeapons = 0;
	Timer switchT;
	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {
		ComponentHandle<WeaponStats> stats;
		ComponentHandle<Equipment> equip;
		ComponentHandle<PlayerComponent> player;
		ComponentHandle<Weapon> weapon;
		ComponentHandle<Transform> trans;

		int playerCount = 0;
		for (Entity entity : es.entities_with_components(equip, trans)) {
			equip = entity.component<Equipment>();
			trans = entity.component<Transform>();


			player = entity.component<PlayerComponent>();

			for (int i = 0; i < equip->slots.size(); i++) {
				Weapon* weapon = &equip->slots[i];
				time_t current = time(NULL);
				if (weapon->playerOwned && (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER)) && playerActiveWeapon == playerCount && weapon->timer.elapsed() > weapon->stats->cooldown && !(weapon->stats->ammo <= 0 && !weapon->stats->infAmmo)) {
					weapon->shouldFire = true;
				}
				if (weapon->shouldFire) {
					weapon->shouldFire = false;
					weapon->timer.restart();

					//spawn bullet/missile/bomb at trans, 
					entityx::Entity projectile = es.create();
					projectile.assign<Transform>(trans->pos + glm::toMat3(trans->orientation) * weapon->offset, trans->orientation);
					projectile.assign<Physics>(0.2, 1, 1, glm::toMat3(trans->orientation) * glm::vec3(0.0, 0.0, weapon->stats->speed), glm::vec3());
					projectile.assign<ModelComponent>(weapon->model);

					weapon->stats->ammo--;
				}
				if (weapon->playerOwned)
					playerCount++;
			}
			
		}

		if (playerCount > nrPlayerWeapons)
			nrPlayerWeapons = playerCount;

		if ((Input::isKeyDown(GLFW_KEY_F2) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_DPAD_DOWN)) && switchT.elapsed() > 0.2f) {
			playerActiveWeapon = (playerActiveWeapon + 1) % nrPlayerWeapons;
			std::cout << "Selected weapon: " << playerActiveWeapon << "\n";
			switchT.restart();
		}
	};
};