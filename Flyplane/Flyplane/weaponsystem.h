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
#include "projectilecomponent.h"
#include <ctime>

using namespace entityx;

struct WeaponSystem : public entityx::System<WeaponSystem> {
	Timer switchT;
	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {
		ComponentHandle<WeaponStats> stats;
		ComponentHandle<Equipment> equip;
		ComponentHandle<PlayerComponent> player;
		ComponentHandle<Weapon> weapon;
		ComponentHandle<Transform> trans;
		ComponentHandle<Projectile> projectile;

		for (Entity entity : es.entities_with_components(equip, trans)) {
			equip = entity.component<Equipment>();
			trans = entity.component<Transform>();
			ComponentHandle<Physics> physics;
			physics = entity.component<Physics>();

			glm::vec3 planeSpeed;
			if (physics) {
				planeSpeed = physics->velocity;
			}

			player = entity.component<PlayerComponent>();

			for (int i = 0; i < equip->slots.size(); i++) {
				Weapon* weapon = &equip->slots[equip->selected];
				time_t current = time(NULL);
				if (player && (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER)) && weapon->timer.elapsed() > weapon->stats->cooldown && weapon->stats->ammo >= 0) {
					weapon->shouldFire = true;
				}
				if (weapon->shouldFire) {
					weapon->shouldFire = false;
					weapon->timer.restart();

					//spawn bullet/missile/bomb at trans, 
					entityx::Entity projectile = es.create();
					projectile.assign<Transform>(trans->pos + glm::toMat3(trans->orientation) * weapon->offset, trans->orientation);
					projectile.assign<Physics>(0.2, 1, glm::toMat3(trans->orientation) * (glm::vec3(0.0, 0.0, weapon->stats->speed) + planeSpeed), glm::vec3());
					projectile.assign<ModelComponent>(weapon->projectileModel);
					projectile.assign<Projectile>(weapon->stats->lifetime);

					if(!weapon->stats->infAmmo)
						weapon->stats->ammo--;
				}
			}

			if ((Input::isKeyDown(GLFW_KEY_F2) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_DPAD_DOWN)) && switchT.elapsed() > 0.2f) {
				equip->selected = (equip->selected + 1) % equip->slots.size();
				std::cout << "Selected weapon: " << equip->selected << ", input: " << Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_DPAD_DOWN) << "\n";
				switchT.restart();
			}
			
		}

		for (Entity entity : es.entities_with_components(projectile)) {
			projectile = entity.component<Projectile>();
			if (projectile->timer.elapsed() > projectile->lifetime)
				entity.destroy();

		}

		
	};
};