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
#include "targetcomponent.h"
#include "window.h"
#include "healthcomponent.h"
#include "factioncomponents.h"
#include "particlecomponent.h"
#include "cameraoncomponent.h"
#include <SFML\System.hpp>

using namespace entityx;

struct WeaponSystem : public entityx::System<WeaponSystem> {
	Timer switchT;

	void turretSpawnBullet(Transform* trans, Turret turret, glm::vec3 planeSpeed, entityx::EntityManager &es, unsigned int parentFaction, float timeOffset = 0.f) {
		entityx::Entity projectile = es.create();


		glm::vec3 dir = trans->orientation * turret.getGunOrientation() * glm::vec3(0.0, 0.0, 1.0);
		glm::vec3 randvec = glm::normalize(glm::vec3(rand() % 10 - 5, (rand() % 10) - 5, (rand() % 10) - 5));
		glm::vec3 velocity = turret.stats.speed * glm::normalize(dir + randvec * 0.01f);
		glm::vec3 pos = trans->pos + (trans->orientation * turret.placement.offset) + (trans->orientation * turret.getGunOrientation() * turret.weapon.projectileOffset);
		pos += velocity * timeOffset;
		velocity += planeSpeed;


		projectile.assign<Transform>(pos, trans->orientation * turret.getGunOrientation(), turret.weapon.projScale);
		projectile.assign<Physics>(turret.stats.mass, 0, velocity, glm::vec3());
		projectile.component<Physics>()->gravity = false;
		//projectile.assign<ModelComponent>(turret.weapon.projModel);
		projectile.assign<Projectile>(turret.stats.lifetime, parentFaction, turret.stats.damage);
		projectile.assign<CollisionComponent>();

		if (parentFaction == FACTION_PLAYER)
			projectile.assign<FactionPlayer>();
		else
			projectile.assign<FactionEnemy>();
		//projectile.assign<SoundComponent>(machinegunSB, false);
	}

	void spawnBullet(Transform* trans, Weapon* weapon, glm::vec3 planeSpeed, entityx::EntityManager &es, unsigned int parentFaction, float timeOffset = 0.f) {
		//std::cout << "Spawning bullet!\n";
		entityx::Entity projectile = es.create();
		//std::cout << weapon->projScale.x << " : " << weapon->projScale.y << " : " << weapon->projScale.z << "\n";


		glm::vec3 dir = trans->orientation * glm::vec3(0.0, 0.0, 1.0);
		glm::vec3 randvec = glm::normalize(glm::vec3(rand()%20 - 10, (rand() % 20) - 10, (rand() % 20) - 10));
		glm::vec3 velocity = weapon->stats.speed * glm::normalize(dir + randvec * 0.01f);
		glm::vec3 pos = trans->pos + trans->orientation * weapon->offset;
		pos += velocity * timeOffset;
		velocity += planeSpeed;


		projectile.assign<Transform>(pos, trans->orientation, weapon->projScale);
		projectile.assign<Physics>(weapon->stats.mass, 0, velocity, glm::vec3());
		projectile.component<Physics>()->gravity = false;
		//projectile.assign<ModelComponent>(weapon->projectileModel);
		projectile.assign<Projectile>(weapon->stats.lifetime, parentFaction, weapon->stats.damage);
		projectile.assign<CollisionComponent>();
		if (parentFaction == FACTION_PLAYER)
			projectile.assign<FactionPlayer>();
		else
			projectile.assign<FactionEnemy>();
		//projectile.assign<SoundComponent>(machinegunSB, false);
	}

	void spawnMissile(Transform* trans, Weapon* weapon, glm::vec3 planeSpeed, entityx::EntityManager &es, entityx::EventManager &em, unsigned int parentFaction) {
		entityx::Entity missile = es.create();
		missile.assign<Transform>(trans->pos + glm::toMat3(trans->orientation) * weapon->offset, trans->orientation, weapon->scale);
		missile.assign<Physics>(weapon->stats.mass, 0, planeSpeed+glm::vec3(0,-10,0), glm::vec3());
		missile.assign<ModelComponent>(weapon->projectileModel);
		missile.assign<Projectile>(weapon->stats.lifetime, parentFaction, weapon->stats.damage);
		missile.assign<Missile>(trans, weapon->stats.speed, weapon->stats.turnRate, weapon->stats.detonateRange, weapon->stats.explodeRadius, weapon->stats.damage, weapon->stats.droptime);
		missile.assign<CollisionComponent>();
		missile.assign<SoundComponent>(*AssetLoader::getLoader().getSoundBuffer("missile"), 100);
		auto handle = missile.assign<ParticleComponent>();
		em.emit<AddParticleEvent>(MISSILE_TRAIL, handle);
	}

	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {
		ComponentHandle<Target> T;
		for (Entity TE : es.entities_with_components(T)) {
			T = TE.component<Target>();
			T->is_targeted = false;
		}
		ComponentHandle<WeaponStats> stats;
		ComponentHandle<Equipment> equip;
		ComponentHandle<Target> target;
		ComponentHandle<PlayerComponent> player;
		ComponentHandle<Weapon> weapon;
		ComponentHandle<Transform> trans;
		ComponentHandle<Projectile> projectile;
		ComponentHandle<Missile> missile;
		ComponentHandle<Physics> physics;
		ComponentHandle<BurstSoundComponent> burstSound;

		for (Entity entity : es.entities_with_components(equip, trans)) {
			equip = entity.component<Equipment>();
			trans = entity.component<Transform>();
			ComponentHandle<Physics> physics;
			physics = entity.component<Physics>();
			target = entity.component<Target>();

			glm::vec3 planeSpeed;
			if (physics) {
				planeSpeed = physics->velocity;
			}

			player = entity.component<PlayerComponent>();

			//turret code start
			for (int i = 0; i < equip->turrets.size(); i++) {

				Entity entity_closest;
				float closest = -1.1f;
				float new_closest = 0.f;

				ComponentHandle<Transform> closest_transform;
				ComponentHandle<Target> closest_target;
				ComponentHandle<ModelComponent> closest_model;

				for (Entity entity_closest_search : es.entities_with_components(closest_target, closest_transform, closest_model)) {
					if (entity_closest_search.has_component<Target>() && entity.has_component<Target>()) {
						if (entity_closest_search != entity && entity_closest_search.component<Target>()->faction != entity.component<Target>()->faction) {
							glm::vec3 interdictionTest = SAIB::ADVInterdiction(entity_closest_search, entity, equip->turrets.at(i).stats.speed, equip->turrets.at(i).placement.offset, dt);
							glm::vec3 gunOrientation = glm::normalize(trans->orientation * equip->turrets.at(i).getGunOrientation() * glm::vec3(0.f, 0.f, 1.f));
							new_closest = glm::dot(glm::normalize(interdictionTest - trans->pos), gunOrientation);

							if (new_closest > closest) {//check if target is inside frustum

								glm::vec2 traverseLimits = equip->turrets.at(i).info.traverseLimit;
								glm::vec2 elevationLimits = equip->turrets.at(i).info.elevationLimit;

								glm::vec3 turretFront = glm::normalize(trans->orientation * equip->turrets.at(i).placement.orientation * glm::vec3(0.f, 0.f, 1.f));
								glm::vec3 turretUp = glm::normalize(trans->orientation * equip->turrets.at(i).placement.orientation * glm::vec3(0.f, 1.f, 0.f));
								glm::vec3 turretLeft = glm::normalize(trans->orientation * equip->turrets.at(i).placement.orientation * glm::vec3(1.f, 0.f, 0.f));

								glm::vec3 pt = glm::normalize(interdictionTest - trans->pos);

								glm::vec3 pt_trav = glm::normalize(pt - glm::proj(pt, turretUp));
								//std::cout << pt.x << " : " << pt.y << " : " << pt.z << "	" << pt_trav.x << " : " << pt_trav.y << " : " << pt_trav.z << "\n";
								float LRC = glm::degrees(glm::acos(glm::dot(pt_trav, turretLeft)));
								float traverseAngle = glm::degrees(glm::acos(glm::dot(pt_trav, turretFront)));
								float traverseLimit;
								if (LRC < 90) {//left
									traverseLimit = traverseLimits.x;
								}
								else {//right
									traverseLimit = traverseLimits.y;
								}

								glm::vec3 pt_elev = glm::normalize(pt - glm::proj(pt, turretLeft));

								float UDC = glm::degrees(glm::acos(glm::dot(pt_elev, turretUp)));
								glm::vec3 adjTurretFront = pt - glm::proj(pt, turretUp);
								float elevationAngle = glm::degrees(glm::acos(glm::dot(pt_elev, adjTurretFront)));
								float elevationLimit;
								if (UDC < 90) {//up
									elevationLimit = elevationLimits.x;
								}
								else {//down
									elevationLimit = elevationLimits.y;
								}
								//std::cout << traverseAngle << " vs " << traverseLimit << " ::::: " << elevationAngle << " vs " << elevationLimit << "\n";
								if (traverseAngle < traverseLimit && elevationAngle < elevationLimit && glm::length(interdictionTest - trans->pos) < equip->turrets.at(i).info.range) {
									entity_closest = entity_closest_search;
									closest = new_closest;
								}
							}
						}
					}
				}
				if (player) {
					equip->turrets.at(i).shouldFire = false;
					if (!equip->turrets.at(i).autoFire && (Input::isKeyDown(GLFW_KEY_LEFT_CONTROL) || Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER))) {
						equip->turrets.at(i).shouldFire = true;
						equip->turrets.at(i).timer.restart();
					}
					
				}

				if (entity_closest.valid()) {
					glm::vec3 interdiction = SAIB::ADVInterdiction(entity_closest, entity, equip->turrets.at(i).stats.speed, equip->turrets.at(i).placement.offset, dt);

					glm::vec2 rotation = SAIB::turretRotateTowards(equip->turrets.at(i), interdiction, trans->orientation, trans->pos);

					equip->turrets.at(i).info.ET += rotation * glm::radians(equip->turrets.at(i).info.turnrate) * float(dt);

					glm::vec3 turret_aim = glm::normalize(trans->orientation * equip->turrets.at(i).getGunOrientation() * glm::vec3(0.f, 0.f, 1.f));
					float to_aim = glm::dot(turret_aim, glm::normalize(interdiction - trans->pos));
					//std::cout << to_aim << "\n";

					if (equip->turrets.at(i).autoFire && to_aim > 0.999f) {
						equip->turrets.at(i).shouldFire = true;
						equip->turrets.at(i).timer.restart();
					} else if (equip->turrets.at(i).autoFire) {
						equip->turrets.at(i).shouldFire = false;
					}
				}
				else {
					glm::vec3 aim = trans->pos + glm::normalize(trans->orientation * equip->turrets.at(i).placement.orientation * glm::vec3(0.f, 0.f, 1.f));
					glm::vec2 rotation = SAIB::turretRotateTowards(equip->turrets.at(i), aim, trans->orientation, trans->pos);
					equip->turrets.at(i).info.ET += rotation * glm::radians(equip->turrets.at(i).info.turnrate) * float(dt);
				}

				unsigned int parentfaction = FACTION_PLAYER;

				if (target) {
					parentfaction = target->faction;
					if (equip->turrets.at(i).shouldFire && equip->turrets.at(i).stats.ammo > 0) {
						equip->turrets.at(i).timeAccum += dt;

						while (equip->turrets.at(i).timeAccum > equip->turrets.at(i).stats.cooldown && equip->turrets.at(i).stats.ammo > 0) {
							equip->turrets.at(i).timeAccum -= equip->turrets.at(i).stats.cooldown;

							turretSpawnBullet(trans.get(), equip->turrets.at(i), planeSpeed, es, parentfaction, equip->turrets.at(i).timeAccum);
							burstSound = entity.component<BurstSoundComponent>();
							if (burstSound) {
								BurstSoundComponent* s = burstSound.get();

								for (auto& sound : s->sounds) {
									if (sound.getStatus() != sound.Playing) {
										sound.play();
										sound.setPlayingOffset(sf::seconds(equip->turrets.at(i).timeAccum));
										break;
									}
								}
							}
						}
					}
				}
				glm::vec2 g = equip->turrets.at(i).info.ET;
				if (g.y < 0) {//up
					if (-glm::degrees(g.y) > equip->turrets.at(i).info.elevationLimit.x) {
						g.y = -glm::radians(equip->turrets.at(i).info.elevationLimit.x);
					}
				}
				else {//down
					if (glm::degrees(g.y) > equip->turrets.at(i).info.elevationLimit.y) {
						g.y = glm::radians(equip->turrets.at(i).info.elevationLimit.y);
					}
				}
				if (equip->turrets.at(i).info.traverseLimit.x != equip->turrets.at(i).info.traverseLimit.x || equip->turrets.at(i).info.traverseLimit.x != 180.f) {
					if (g.x < 0) {//right
						if (-glm::degrees(g.x) > equip->turrets.at(i).info.traverseLimit.y) {
							g.x = -glm::radians(equip->turrets.at(i).info.traverseLimit.y);
						}
					}
					else {//left
						if (glm::degrees(g.x) > equip->turrets.at(i).info.traverseLimit.x) {
							g.x = glm::radians(equip->turrets.at(i).info.traverseLimit.x);
						}
					}
				}
				equip->turrets.at(i).info.ET = g;
			}
			//turret code end


			Weapon* weapon = nullptr;
			if (equip->special.size() > 0) {
				weapon = &equip->special[equip->selected];
				if (player && (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT) || Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_LEFT_BUMPER)) && weapon->stats.ammo > 0) {
					if (weapon->timer.elapsed() > weapon->stats.cooldown)
						weapon->shouldFire = true;
				}
			}
			
			
			for (int i = 0; i < equip->primary.size(); i++) {
				Weapon* pweapon = &equip->primary[i];
				if (player && (Input::isKeyDown(GLFW_KEY_LEFT_CONTROL) || Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER))) {
					pweapon->shouldFire = true;
					//std::cout << "FIRING!\n";
				} else if (player) {
					pweapon->shouldFire = false;
				}
				if(pweapon->timeAccum < pweapon->stats.cooldown)
					pweapon->timeAccum += dt;

				if (pweapon->shouldFire && pweapon->stats.ammo > 0) {

					while (pweapon->timeAccum > 0 && pweapon->stats.ammo > 0) {
						
						pweapon->shouldFire = false;
						pweapon->timer.restart();

						unsigned int parentfaction = FACTION_PLAYER;

						if (target)
							parentfaction = target->faction;

						if (pweapon->isMissile)
							spawnMissile(trans.get(), pweapon, planeSpeed, es, events, parentfaction);
						else {
							spawnBullet(trans.get(), pweapon, planeSpeed, es, parentfaction, pweapon->timeAccum);

							burstSound = entity.component<BurstSoundComponent>();
							if (burstSound) {
								BurstSoundComponent* s = burstSound.get();

								bool found = false;
								for (auto& sound : s->sounds) {
									if (sound.getStatus() != sound.Playing) {
										sound.play();
										sound.setPlayingOffset(sf::seconds(pweapon->timeAccum));
										found = true;
										break;
									}
								}
								if (!found) {
									//std::cout << "Could not find non playing sound for weapon!\n";
								}
								/*
								if (s->sound.getStatus() != s->sound.Playing) {
									s->sound.play();
								}
								*/
							}

							auto cameraOn = entity.component<CameraOnComponent>();
							if (cameraOn) {
								cameraOn->shake += 2.f*pweapon->stats.cooldown;
							}
						}

						pweapon->timeAccum -= pweapon->stats.cooldown;
					}
				}
			}
			
			if ((Input::isKeyDown(GLFW_KEY_Q) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_DPAD_DOWN)) && switchT.elapsed() > 0.2f && equip->special.size() > 0) {
				Weapon lastWep = equip->special[equip->selected];
				equip->selected = (equip->selected + 1) % equip->special.size();
				unsigned int count = 0;
				while (count < equip->special.size() - 1 && (equip->special[equip->selected].model == lastWep.model || equip->special[equip->selected].stats.ammo <= 0)) {
					equip->selected = (equip->selected + 1) % equip->special.size();
					count++;
				}
				switchT.restart();
			}

			if (equip->special.size() > 0 && weapon->shouldFire && weapon->timer.elapsed() > weapon->stats.cooldown) {
				weapon->shouldFire = false;
				weapon->timer.restart();

				unsigned int parentfaction = FACTION_PLAYER;
				if (target)
					parentfaction = target->faction;

				if (weapon->isMissile)
					spawnMissile(trans.get(), weapon, planeSpeed, es,events, parentfaction);
				else
					spawnBullet(trans.get(), weapon, planeSpeed, es, parentfaction);

				if (!weapon->stats.infAmmo)
					weapon->stats.ammo--;

				int preselect = equip->selected;

				if (weapon->dissappear && weapon->stats.ammo <= 0) {
					equip->removeSpecialWeapon();
					//equip->special.erase(equip->special.begin() + equip->selected);
					/*equip->selected = 0;
					equip->special[equip->selected].timer.restart();*/
				}

				int max = equip->special.size();
				int c = 0;
				if (equip->special.size() > 0) {
					while (equip->special[equip->selected].stats.ammo <= 0 && c <= max) {
						equip->selected = (equip->selected + 1) % equip->special.size();
						c++;
					}
				}
				equip->special[equip->selected].timer.restart();
			}

			//Weapon lastWep = equip->special[equip->selected];
			unsigned int count = 0;
			unsigned int totalAmmo = 0;
			unsigned int tempselect = equip->selected;
			while (count < equip->special.size()) {
				if(equip->special[equip->selected].model == equip->special[tempselect].model)
					totalAmmo += equip->special[tempselect].stats.ammo;
				tempselect = (tempselect + 1) % equip->special.size();
				count++;
			}


			
			
		}

		for (Entity entity : es.entities_with_components(projectile)) {
			projectile = entity.component<Projectile>();
			if (projectile->timer.elapsed() > projectile->lifetime)
				entity.destroy();
		}

		entityx::ComponentHandle<PlayerComponent> play;
		for (Entity entity : es.entities_with_components(play, trans)) {
			entityx::ComponentHandle<Target> target;
			entityx::ComponentHandle<Transform> aitran;
			glm::vec3 v = glm::toMat3(trans->orientation) * glm::vec3(0.0, 0.0, 10.0);
			float bestDot = -1;
			double bestScore = -1;
			Entity cure;
			for (Entity enemy : es.entities_with_components(aitran, target)) {
				glm::vec3 dir = aitran->pos - trans->pos;
				float dot = glm::dot(glm::normalize(dir), glm::normalize(v));
				target->preview_target = false;
				double score = (dot * target->heat) / glm::length(dir);
				if (score > bestScore && entity.has_component<Target>() && entity.component<Target>()->faction != target->faction) {
					bestDot = dot;
					bestScore = score;
					cure = enemy;
				}
			}
			bool noTarget = false;
			glm::vec3 forward = v;
			Transform newTrans;
			newTrans.pos = forward;

			if (bestDot == -1 || bestDot < 0.2) {
				noTarget = true;
			}

			if (cure.valid() && !noTarget)
				cure.component<Target>()->preview_target = true;
		}
		


		entityx::ComponentHandle<AIComponent> ai;
		
		entityx::ComponentHandle<FlightComponent> aiflight;
		for (Entity entity : es.entities_with_components(missile, trans, physics, projectile)) {
			missile = entity.component<Missile>();
			trans = entity.component<Transform>();
			physics = entity.component<Physics>();
			projectile = entity.component<Projectile>();
			if (projectile->timer.elapsed() > missile->droptime) {
				glm::vec3 v = glm::toMat3(trans->orientation) * glm::vec3(0.0, 0.0, 10.0);
				float bestDot = -1;
				double bestScore = -1;
				Entity cure;
				entityx::ComponentHandle<Target> target;
				entityx::ComponentHandle<Transform> aitrans;
				//entityx::ComponentHandle<HealthComponent> health;
				//for (Entity enemy : es.entities_with_components(aitrans, target, health)) {
				for (Entity enemy : es.entities_with_components(aitrans, target)) {
					glm::vec3 dir = aitrans->pos - trans->pos;
					float dot = glm::dot(glm::normalize(dir), glm::normalize(v));
					ai = enemy.component<AIComponent>();
					double score = (dot * target->heat) / glm::length(dir);
					if (score > bestScore && projectile->parentFaction != target->faction) {
						bestDot = dot;
						bestScore = score;
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

				if (cure.valid() && !noTarget) {
					cure.component<Target>()->is_targeted = true;
				}
					

				glm::quat q;

				
				glm::vec3 u = missile->target->pos - trans->pos;
				glm::vec3 vn = glm::normalize(v);
				glm::vec3 un = glm::normalize(u);
				glm::vec3 cross = glm::cross(vn, un);


				float turnRate = missile->turnRate;

				
				if (!noTarget) {
					q = glm::angleAxis((float)(turnRate * dt), glm::normalize(cross));
					trans->orientation = q * trans->orientation;
					trans->orientation = glm::normalize(trans->orientation);
				}
				
				//sstd::cout << "Missile position: " << trans->pos.x << " " << trans->pos.y << " " << trans->pos.z << "dot: " << glm::dot(vn, un) << "\n";
				physics->velocity = glm::toMat3(trans->orientation) * glm::vec3(0,0,missile->speed);

				if (glm::length(u) < missile->detonateRange) {
					missile->shouldExplode = true;
				}
			}
			if (missile->shouldExplode) {
				std::cout << "Missile exploded \n";
				Entity explosion = es.create();
				explosion.assign<ExplosionComponent>(missile->explodeDamage, missile->explodeRadius);
				explosion.assign<Transform>(trans->pos);
				explosion.assign<BurstSoundComponent>(*AssetLoader::getLoader().getSoundBuffer("explosion"), trans->pos, true, missile->explodeRadius, 0.01, 1, 100);
				auto handle = explosion.assign<ParticleComponent>();
				ParticleParameters params;
				params.effectLength = 3.f;
				params.explosion.radius = missile->explodeRadius;
				events.emit<AddParticleEvent>(EXPLOSION, handle, params);
				entity.destroy();
			}
		}

		//explosions
		entityx::ComponentHandle<ExplosionComponent> explosion;
		for (Entity entity_explosion : es.entities_with_components(explosion, trans)) {
			entityx::ComponentHandle<Target> target;
			entityx::ComponentHandle<HealthComponent> health;
			entityx::ComponentHandle<Transform> tran1;
			for (Entity entity : es.entities_with_components(health, tran1)) {
				double dist = glm::length(trans->pos - tran1->pos);
				double fallof = explosion->damage / explosion->radius;
				double damage = explosion->damage - (dist*fallof);
				if (damage < 0)
					damage = 0;
				health->health -= damage;
				std::cout << "Explosion did " << damage << " damage." << "\n";
			}
			entity_explosion.remove<ExplosionComponent>();
		}
	};
};