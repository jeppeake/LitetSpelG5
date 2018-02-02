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
#include "collisioncomponent.h"
#include "aicomponent.h"
#include "soundcomponent.h"
#include "flightcomponent.h"
#include <glm/gtx/vector_angle.hpp>
#include <ctime>
#include "soundbuffers.h"


using namespace entityx;

struct WeaponSystem : public entityx::System<WeaponSystem> {
	Timer switchT;

	void spawnBullet(Transform* trans, Weapon* weapon, glm::vec3 planeSpeed, entityx::EntityManager &es) {
		entityx::Entity projectile = es.create();
		projectile.assign<Transform>(trans->pos + glm::toMat3(trans->orientation) * weapon->offset, trans->orientation, weapon->projScale);
		projectile.assign<Physics>(weapon->stats.mass, 1, glm::toMat3(trans->orientation) * glm::vec3(0.0, 0.0, weapon->stats.speed) + planeSpeed, glm::vec3());
		projectile.assign<ModelComponent>(weapon->projectileModel);
		projectile.assign<Projectile>(weapon->stats.lifetime);
		projectile.assign<CollisionComponent>();
		//projectile.assign<SoundComponent>(machinegunSB, false);
	}

	void spawnMissile(Transform* trans, Weapon* weapon, glm::vec3 planeSpeed, entityx::EntityManager &es) {
		entityx::Entity missile = es.create();
		missile.assign<Transform>(trans->pos + glm::toMat3(trans->orientation) * weapon->offset, trans->orientation, weapon->scale);
		missile.assign<Physics>(weapon->stats.mass, 1, planeSpeed+glm::vec3(0,-10,0), glm::vec3());
		missile.assign<ModelComponent>(weapon->projectileModel);
		missile.assign<Projectile>(weapon->stats.lifetime);
		missile.assign<Missile>(trans);
		missile.assign<CollisionComponent>();
		missile.assign<SoundComponent>(missileSB);
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

			if (player && (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT) || Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_LEFT_BUMPER)) && weapon->timer.elapsed() > weapon->stats.cooldown && weapon->stats.ammo > 0) {
				weapon->shouldFire = true;
			}
			
			for (int i = 0; i < equip->primary.size(); i++) {
				Weapon* pweapon = &equip->primary[i];
				if (player && (Input::isKeyDown(GLFW_KEY_LEFT_CONTROL) || Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER)) && pweapon->timer.elapsed() > pweapon->stats.cooldown && pweapon->stats.ammo > 0) {
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
				switchT.restart();
			}

			if (weapon->shouldFire) {
				weapon->shouldFire = false;
				weapon->timer.restart();

				if (weapon->isMissile)
					spawnMissile(trans.get(), weapon, planeSpeed, es);
				else
					spawnBullet(trans.get(), weapon, planeSpeed, es);

				if (!weapon->stats.infAmmo)
					weapon->stats.ammo--;

				if (weapon->dissappear && weapon->stats.ammo <= 0) {
					equip->special.erase(equip->special.begin() + equip->selected);
					/*equip->selected = 0;
					equip->special[equip->selected].timer.restart();*/
				}

				int max = equip->special.size();
				int c = 0;
				while (equip->special[equip->selected].stats.ammo <= 0 && c <= max) {
					equip->selected++;
					c++;
				}
				equip->special[equip->selected].timer.restart();
			}
			
		}

		for (Entity entity : es.entities_with_components(projectile)) {
			projectile = entity.component<Projectile>();
			if (projectile->timer.elapsed() > projectile->lifetime)
				entity.destroy();
		}


		entityx::ComponentHandle<AIComponent> ai;
		entityx::ComponentHandle<Transform> aitrans;
		entityx::ComponentHandle<FlightComponent> aiflight;
		for (Entity entity : es.entities_with_components(missile, trans, physics, projectile)) {
			missile = entity.component<Missile>();
			trans = entity.component<Transform>();
			physics = entity.component<Physics>();
			projectile = entity.component<Projectile>();
			if (projectile->timer.elapsed() > 1) {
				glm::vec3 v = glm::toMat3(trans->orientation) * glm::vec3(0.0, 0.0, 10.0);
				float bestDot = -1;
				Entity cure;
				for (Entity enemy : es.entities_with_components(ai, aitrans)) {
					glm::vec3 dir = aitrans->pos - trans->pos;
					float dot = glm::dot(glm::normalize(dir), glm::normalize(v));
					ai->is_targeted = false;
					if (dot > bestDot) {
						bestDot = dot;
						missile->target = aitrans.get();
						cure = enemy;
					}
				}

				bool noTarget = false;
				glm::vec3 forward = v;
				Transform newTrans;
				newTrans.pos = forward;

				if (bestDot == -1 || bestDot < 0.2) {
					missile->target = &newTrans;
					noTarget = true;
				}

				if (cure.valid() && !noTarget)
					cure.component<AIComponent>()->is_targeted = true;
					

				glm::quat q;

				
				glm::vec3 u = missile->target->pos - trans->pos;
				glm::vec3 vn = glm::normalize(v);
				glm::vec3 un = glm::normalize(u);
				glm::vec3 cross = glm::cross(vn, un);


				float turnRate = 3.f;

				
				if (!noTarget) {
					q = glm::angleAxis((float)(turnRate * dt), glm::normalize(cross));
					trans->orientation = q * trans->orientation;
					trans->orientation = glm::normalize(trans->orientation);
				}
				
				//sstd::cout << "Missile position: " << trans->pos.x << " " << trans->pos.y << " " << trans->pos.z << "dot: " << glm::dot(vn, un) << "\n";
				physics->velocity = glm::toMat3(trans->orientation) * glm::vec3(0,0,300);

				if (glm::length(u) < 10.0) {
					std::cout << "Missile hit target at: " << " " << u.x << " " << u.y << " " << glm::length(u) << "\n";
					if (!noTarget) {
						if (cure.valid()) {
							cure.destroy();
						}
						entity.destroy();
					}
				}
			}
				
		}
	};
};