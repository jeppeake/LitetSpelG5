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
#include "missilecomponent.h"
#include <ctime>


using namespace entityx;

struct WeaponSystem : public entityx::System<WeaponSystem> {
	Timer switchT;

	void spawnBullet(Transform* trans, Weapon* weapon, glm::vec3 planeSpeed, entityx::EntityManager &es) {
		entityx::Entity projectile = es.create();
		projectile.assign<Transform>(trans->pos + glm::toMat3(trans->orientation) * weapon->offset, trans->orientation);
		projectile.assign<Physics>(weapon->stats.mass, 1, glm::toMat3(trans->orientation) * glm::vec3(0.0, 0.0, weapon->stats.speed) + planeSpeed, glm::vec3());
		projectile.assign<ModelComponent>(weapon->projectileModel);
		projectile.assign<Projectile>(weapon->stats.lifetime);
	}

	void spawnMissile(Transform* trans, Weapon* weapon, glm::vec3 planeSpeed, entityx::EntityManager &es) {
		entityx::Entity missile = es.create();
		missile.assign<Transform>(trans->pos + glm::toMat3(trans->orientation) * weapon->offset, trans->orientation);
		missile.assign<Physics>(weapon->stats.mass, 1, planeSpeed, glm::vec3());
		missile.assign<ModelComponent>(weapon->projectileModel);
		missile.assign<Projectile>(weapon->stats.lifetime);
		missile.assign<Missile>(glm::vec3(0, 0, 0));
	}

	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {
		ComponentHandle<WeaponStats> stats;
		ComponentHandle<Equipment> equip;
		ComponentHandle<PlayerComponent> player;
		ComponentHandle<Weapon> weapon;
		ComponentHandle<Transform> trans;
		ComponentHandle<Projectile> projectile;
		ComponentHandle<Missile> missile;
		ComponentHandle<Physics> physics;

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

			Weapon* weapon = &equip->special[equip->selected];
			std::cout << "selectd weapon ammo: " << weapon->stats.ammo << "\n";

			if (player && (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT) || Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER)) && weapon->timer.elapsed() > weapon->stats.cooldown && weapon->stats.ammo > 0) {
				weapon->shouldFire = true;
			}
			
			for (int i = 0; i < equip->primary.size(); i++) {
				Weapon* pweapon = &equip->primary[i];
				if (player && (Input::isKeyDown(GLFW_KEY_LEFT_CONTROL) || Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_LEFT_BUMPER)) && pweapon->timer.elapsed() > pweapon->stats.cooldown && pweapon->stats.ammo > 0) {
					pweapon->shouldFire = true;
				}
				if (pweapon->shouldFire) {
					pweapon->shouldFire = false;
					pweapon->timer.restart();
					if(pweapon->isMissile)
						spawnMissile(trans.get(), pweapon, planeSpeed, es);
					else
						spawnBullet(trans.get(), pweapon, planeSpeed, es);
				}
			}

			if ((Input::isKeyDown(GLFW_KEY_F2) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_DPAD_DOWN)) && switchT.elapsed() > 0.2f) {
				equip->selected = (equip->selected + 1) % equip->special.size();
				std::cout << "Selected weapon: " << equip->selected << ", input: " << Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_DPAD_DOWN) << "\n";
				switchT.restart();
			}

			if (weapon->shouldFire) {
				weapon->shouldFire = false;
				weapon->timer.restart();

				spawnBullet(trans.get(), weapon, planeSpeed, es);
				if (weapon->isMissile)
					spawnMissile(trans.get(), weapon, planeSpeed, es);
				else
					spawnBullet(trans.get(), weapon, planeSpeed, es);

				if (!weapon->stats.infAmmo)
					weapon->stats.ammo--;

				if (weapon->dissappear && weapon->stats.ammo <= 0) {
					equip->special.erase(equip->special.begin() + equip->selected);
					std::cout << equip->special.size() << "\n";
					equip->selected = 0;
					equip->special[0].timer.restart();
				}
			}
			
		}

		for (Entity entity : es.entities_with_components(projectile)) {
			projectile = entity.component<Projectile>();
			if (projectile->timer.elapsed() > projectile->lifetime)
				entity.destroy();
		}


		for (Entity entity : es.entities_with_components(missile, trans, physics, projectile)) {
			if (projectile->timer.elapsed() > 1) {
				physics->velocity = glm::toMat3(trans->orientation) * glm::vec3(0.0, 0.0, 400.0);
				//do guided missile shit
			}
				
		}
	};
};