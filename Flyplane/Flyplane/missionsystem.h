#pragma once
#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "mission.h"
#include "playingstate.h"
#include <experimental/filesystem>
#include "housecomponent.h"
#include "missionmarker.h"
#include "huntStatic.h"

#include <iomanip>
#include <cmath>
#include <limits>
namespace fs = std::experimental::filesystem;
using namespace entityx;

struct MissionSystem : public entityx::System<MissionSystem> {
	bool active = false;
	bool failed = false;
	std::vector<Mission> missions;
	unsigned int iActive = 0;
	PlayingState* state;
	Timer timer;
	Timer failTimer;
	double downtime = 10;
	double mTime = 10;
	Entity target;
	Entity formLeader;
	Mission curMission;
	std::vector<Entity> enemyList;

	void cleanUpMarkers() {
		if (formLeader.valid()) {
			if (formLeader.has_component<MissionMarker>()) {
				formLeader.component<MissionMarker>().remove();
			}
		}
		if (target.valid()) {
			if (target.has_component<MissionMarker>()) {
				target.component<MissionMarker>().remove();
			}
		}
		for (Entity enemy : enemyList) {
			if (enemy.valid()) {
				if (enemy.has_component<MissionMarker>()) {
					enemy.component<MissionMarker>().remove();
				}
			}
		}
		enemyList.clear();
	}

	void fail() {
		failTimer.restart();
		std::cout << "Mission failed \n";
		active = false;
		if (formLeader.valid()) {
			if (formLeader.has_component<FormationComponent>()) {
				formLeader.component<FormationComponent>().remove();
			}
		}
		cleanUpMarkers();
		enemyList.clear();
		failed = true;
	}

	MissionSystem(PlayingState *state) : state(state) {
		std::string path = "assets/Presets/Missions";
		
		for (auto & p : fs::directory_iterator(path)) {
			std::string curPath = p.path().string();
			std::cout << "Loading mission: " << curPath << "\n";
			Mission m;
			m.loadMission(curPath);

			for (int i = 0; i < m.enemies.size(); i++) {
				std::ifstream file("assets/Presets/AI/" + m.enemies[0].loadoutFile);
				std::string str;

				PlanePreset pp;
				std::getline(file, str);
				pp.load(str);
				for (int j = 0; j < pp.wepPos.size(); j++) {
					std::getline(file, str);
					if (str.compare("0") != 0) {
						WeaponPreset wp;
						wp.load(str);
					}
				}
			}

			missions.push_back(m);


		}
	}
	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {
		if (active) {
			glClear(GL_DEPTH_BUFFER_BIT);
			glm::vec3 textColor = glm::vec3(1, 0, 0);

			if (curMission.type == MISSIONTYPE_DEFEND) {
				textColor = MARKER_DEFEND;
				
				if (target.valid() && formLeader.valid()) {
					//win condition
					if (formLeader.component<HealthComponent>()->isDead) {
						state->addPoints(curMission.points);
						active = false;
						timer.restart();
						cleanUpMarkers();
					} //fail condition
					if (target.component<HealthComponent>()->isDead) {
						target.destroy();
						fail();
					}
				}
				//leader crashes condition (win)
				else if(!formLeader.valid()) {
					state->addPoints(curMission.points);
					active = false;
					timer.restart();
					cleanUpMarkers();
				}
			}
			else if (curMission.type == MISSIONTYPE_ATTACK) {
				textColor = MARKER_KILL;
				//win condition
				if (target.valid() && state->entity_p.valid()) {
					if(target.component<HealthComponent>()->isDead) {
					//if (glm::distance(target.component<Transform>().get()->pos, state->entity_p.component<Transform>().get()->pos) <= 100) {
						state->addPoints(curMission.points);
						active = false;
						target.destroy();
						timer.restart();
						cleanUpMarkers();
					}
				}
				else {
					fail();
					std::cout << "[BUG] House was destroyed too early \n";
				}
			}
			else if (curMission.type == MISSIONTYPE_KILLALL) {
				textColor = MARKER_KILL;
				int alive = 0;
				for (Entity enemy : enemyList) {
					if (enemy.valid()) {
						if (!enemy.component<HealthComponent>()->isDead) {
							alive++;
						}
						else {
							if (enemy.has_component<MissionMarker>()) {
								enemy.component<MissionMarker>().remove();
							}
						}
					}
				}
				//draw enemies left
				std::string txt = std::to_string(enemyList.size() - alive) + '/' + std::to_string(enemyList.size());
				double x = (Window::getWindow().size().x / 2) - 9 * txt.length() / 2;
				AssetLoader::getLoader().getText()->drawText(txt,
					glm::vec2(x, Window::getWindow().size().y - 260), glm::vec3(1, 0, 0), 0.3);

				//win condition
				if (alive == 0) {
					state->addPoints(curMission.points);
					active = false;
					cleanUpMarkers();
					enemyList.clear();
					timer.restart();
				}
			}
			//draw mission text
			double textSize = 0.6;
			double x = (Window::getWindow().size().x / 2) - 9 * curMission.missiontext.length()/2;
			AssetLoader::getLoader().getMenutext()->drawText(curMission.missiontext,
				glm::vec2(x, Window::getWindow().size().y - 200), textColor, textSize);

			std::ostringstream oss;
			oss << std::setprecision(4) << curMission.time - timer.elapsed();
			std::string str = oss.str();
			std::string ex = "20.55";
			std::string txt = str;
			x = (Window::getWindow().size().x / 2) - 9 * ex.length() / 2;
			AssetLoader::getLoader().getText()->drawText(txt,
				glm::vec2(x, Window::getWindow().size().y - 230), glm::vec3(1, 0, 0), 0.3);
			
			//universal fail condition
			if (timer.elapsed() >= mTime) {
				fail();
			}
		}
		else if (failed) {
			std::string txt = "You failed the mission!";
			double x = (Window::getWindow().size().x / 2) - 9 * txt.length() / 2;
			AssetLoader::getLoader().getText()->drawText(txt,
				glm::vec2(x, Window::getWindow().size().y - 200), glm::vec3(1, 0, 0), 0.3);
			if (failTimer.elapsed() > 5) {
				timer.restart();
				failed = false;
			}
		}
		else {
			if (timer.elapsed() >= downtime) {
				//select a random mission
				int i = rand() % missions.size();
				Mission mi = missions[i];
				curMission = mi;

				//spawn houses
				for (HouseInfo house : missions[i].houses) {
					auto entity = es.create();
					entity.assign<ModelComponent>(house.model);
					glm::vec3 pos = glm::vec3(house.pos.x, AssetLoader::getLoader().getHeightmap("testmap")->heightAt(glm::vec3(house.pos.x, 0.f, house.pos.z)) + 200, house.pos.z);
					if (house.random) {
						pos = pos + glm::vec3((rand() % 8000)-4000, 0, (rand() % 8000)-4000);
						pos.y = AssetLoader::getLoader().getHeightmap("testmap")->heightAt(glm::vec3(pos.x, 0.f, pos.z)) + 40;
					}
					std::cout << "Spawned house at: " << std::to_string(pos.x) << " " << std::to_string(pos.y) << " " << std::to_string(pos.z) << "\n";
					entity.assign<Transform>(pos);
					entity.component<Transform>()->scale = glm::vec3(1.0f);
					
					if (house.condition == CONDITION_DEFEND) {
						entity.assign<Target>(FACTION_PLAYER, 100);
						entity.assign<FactionPlayer>();
						entity.assign<MissionMarker>(MARKER_DEFEND);
					}
					else if (house.condition == CONDITION_DESTROY) {
						entity.assign<Target>(FACTION_AI, 100);
						entity.assign<FactionEnemy>();
						entity.assign<MissionMarker>(MARKER_KILL);
					}
					target = entity;
					entity.assign<HouseComponent>();
					entity.assign<HealthComponent>(300);
					entity.assign<CollisionComponent>();
					
				}

				//spawn leader
				if (mi.formation) {
					EnemyInfo enemy = mi.enemies[mi.iLeader];
					auto entity = es.create();
					float x = enemy.pos.x;
					float z = enemy.pos.z;

					std::ifstream file("assets/Presets/AI/" + enemy.loadoutFile);
					std::string str;

					PlanePreset pp;
					std::getline(file, str);
					pp.load(str);

					glm::vec3 pos(x, AssetLoader::getLoader().getHeightmap("testmap")->heightAt(glm::vec3(x, 0, z)) + enemy.pos.y, z);
					entity.assign<Transform>(pos, glm::quat());
					entity.assign<Physics>(1000.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
					entity.assign<ModelComponent>(AssetLoader::getLoader().getModel(pp.name));
					entity.assign<FlightComponent>(pp.normalspeed, pp.boostspeed, pp.breakforce, pp.turnrate, pp.acceleration);
					entity.assign<Target>(10.0, FACTION_AI);
					entity.assign<FactionEnemy>();
					
					

					auto handle = entity.assign<ParticleComponent>();
					events.emit<AddParticleEvent>(TRAIL, handle);
					events.emit<AddParticleEvent>(ENGINE_TRAIL, handle);

					std::vector<Behaviour*> behaviours;

					entity.assign<FormationComponent>(100);
					entity.assign<MissionMarker>(MARKER_KILL);

					std::vector<glm::vec3> plotter;
					plotter.push_back(glm::vec3(2500, 4500, 0));
					plotter.push_back(glm::vec3(2500, 4500, 2500));
					plotter.push_back(glm::vec3(0, 4500, 2500));
					plotter.push_back(glm::vec3(0, 4500, 0));

					//behaviours.push_back(new Constant_Turn(0));

					//behaviours.push_back(new Follow_Path(1, new Always_True(), plotter, true));
					//behaviours.push_back(new Hunt_Target(2, new Enemy_Close(5000.f), entity_p, 0.05f, 500.f));
					behaviours.push_back(new Hunt_Static(2, new Always_True(), target, 0.05f, 500.f));
					//behaviours.push_back(new Follow_Target(7,new Always_True(), target));
					behaviours.push_back(new Fly_Up(10, new Ground_Close_Front(4.f, 10)));
					behaviours.push_back(new Avoid_Closest(9, new Entity_Close(40.f)));

					entity.assign<AIComponent>(behaviours, true, true, false);
					entity.assign<CollisionComponent>();
					entity.assign<SoundComponent>(*AssetLoader::getLoader().getSoundBuffer("takeoff"));
					entity.assign<BurstSoundComponent>(*AssetLoader::getLoader().getSoundBuffer("machinegun"));

					std::vector<Weapon> weapons;

					for (int i = 0; i < pp.wepPos.size(); i++) {
						std::getline(file, str);
						if (str.compare("0") != 0) {
							WeaponPreset wp;
							wp.load(str);

							AssetLoader::getLoader().loadModel(wp.model, wp.name);
							AssetLoader::getLoader().loadModel(wp.projModel, wp.projModel);

							WeaponStats stats = WeaponStats(wp.ammo, wp.lifetime, wp.speed, wp.mass, wp.cooldown, wp.infAmmo, wp.turnRate, wp.detonateRange, wp.explodeRadius, wp.damage, wp.droptime);

							weapons.emplace_back(stats, AssetLoader::getLoader().getModel(wp.name), AssetLoader::getLoader().getModel(wp.projModel), pp.wepPos[i] + wp.extraOffset, glm::vec3(wp.scale), glm::vec3(wp.projScale), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), wp.isMissile, wp.dissappear);
						}
					}
					std::getline(file, str);
					std::getline(file, str);
					double hp = std::stod(str);
					entity.assign<HealthComponent>(hp);

					WeaponStats MGstats = WeaponStats(10000, 3, 35000, 0.2, 0.02f, true);
					WeaponStats rocketpodstat = WeaponStats(14, 100, 700, 0.2, 0.5f, false);
					std::vector<Weapon> primary;

					primary.emplace_back(MGstats, AssetLoader::getLoader().getModel("gunpod"), AssetLoader::getLoader().getModel("bullet"), glm::vec3(-0.0, -0.5, 1.0), glm::vec3(0.5), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)));
					
					entity.assign<Equipment>(primary, weapons);
					entity.assign<PointComponent>(100);
					formLeader = entity;
					enemyList.push_back(entity);
				}



				//spawn enemies

				int count = 0;
				for (EnemyInfo enemy : missions[i].enemies) {
					if (missions[i].iLeader == count && mi.formation) {
						count++;
						continue;
					}
						
					auto entity = es.create();
					float x = enemy.pos.x;
					float z = enemy.pos.z;

					std::ifstream file("assets/Presets/AI/" + enemy.loadoutFile);
					std::string str;

					PlanePreset pp;
					std::getline(file, str);
					pp.load(str);

					glm::vec3 pos(x, AssetLoader::getLoader().getHeightmap("testmap")->heightAt(glm::vec3(x, 0, z)) + enemy.pos.y, z);
					entity.assign<Transform>(pos, glm::quat());
					entity.assign<Physics>(1000.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
					entity.assign<ModelComponent>(AssetLoader::getLoader().getModel(pp.name));
					entity.assign<FlightComponent>(pp.normalspeed, pp.boostspeed, pp.breakforce, pp.turnrate, pp.acceleration);
					entity.assign<Target>(10.0, FACTION_AI);
					entity.assign<FactionEnemy>();

					auto handle = entity.assign<ParticleComponent>();
					events.emit<AddParticleEvent>(TRAIL, handle);
					events.emit<AddParticleEvent>(ENGINE_TRAIL, handle);
					std::vector<Behaviour*> behaviours;

					

					//behaviours.push_back(new Constant_Turn(0));

					if (mi.type == MISSIONTYPE_ATTACK || mi.type == MISSIONTYPE_KILLALL) {
						std::vector<glm::vec3> plotter;
						glm::vec3 housepos = enemy.pos;
						if (mi.type == MISSIONTYPE_ATTACK) {
							housepos = target.component<Transform>()->pos;
							entity.component<Transform>()->pos = housepos;
							entity.component<Transform>()->pos.y = housepos.y + rand() % 300;
						}
						
						plotter.push_back(glm::vec3(housepos.x + 200, housepos.y + 100, housepos.z + 200));
						plotter.push_back(glm::vec3(housepos.x + 200, housepos.y + 100, housepos.z - 200));
						plotter.push_back(glm::vec3(housepos.x - 200, housepos.y + 100, housepos.z - 200));
						plotter.push_back(glm::vec3(housepos.x - 200, housepos.y + 100, housepos.z + 200));
						behaviours.push_back(new Follow_Path(1, new Always_True(), plotter, true));
					}
					behaviours.push_back(new Hunt_Target(5, new Enemy_Close(mi.huntPlayerDist), state->entity_p, 0.05f, mi.firingDistance));
					//behaviours.push_back(new Hunt_Target(3, new Always_True(), target, 0.05, 500.f));
					behaviours.push_back(new Fly_Up(10, new Ground_Close_Front(4.f, 10)));
					behaviours.push_back(new Avoid_Closest(9, new Entity_Close(40.f)));
					if (mi.formation) {
						behaviours.push_back(new Form_On_Formation(6, new Always_True(), formLeader));
					}

					if (mi.type == MISSIONTYPE_KILLALL) {
						entity.assign<MissionMarker>(MARKER_KILL);
					}
					entity.assign<AIComponent>(behaviours, true, true, false);
					entity.assign<CollisionComponent>();
					entity.assign<SoundComponent>(*AssetLoader::getLoader().getSoundBuffer("takeoff"));
					entity.assign<BurstSoundComponent>(*AssetLoader::getLoader().getSoundBuffer("machinegun"));

					std::vector<Weapon> weapons;

					for (int i = 0; i < pp.wepPos.size(); i++) {
						std::getline(file, str);
						if (str.compare("0") != 0) {
							WeaponPreset wp;
							wp.load(str);

							AssetLoader::getLoader().loadModel(wp.model, wp.name);
							AssetLoader::getLoader().loadModel(wp.projModel, wp.projModel);

							WeaponStats stats = WeaponStats(wp.ammo, wp.lifetime, wp.speed, wp.mass, wp.cooldown, wp.infAmmo, wp.turnRate, wp.detonateRange, wp.explodeRadius, wp.damage, wp.droptime);

							weapons.emplace_back(stats, AssetLoader::getLoader().getModel(wp.name), AssetLoader::getLoader().getModel(wp.projModel), pp.wepPos[i] + wp.extraOffset, glm::vec3(wp.scale), glm::vec3(wp.projScale), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), wp.isMissile, wp.dissappear);
						}
					}

					std::getline(file, str);
					std::getline(file, str);
					double hp = std::stod(str);
					entity.assign<HealthComponent>(hp);

					WeaponStats MGstats = WeaponStats(10000, 3, 35000, 0.2, 0.02f, true);
					WeaponStats rocketpodstat = WeaponStats(14, 100, 700, 0.2, 0.5f, false);
					std::vector<Weapon> primary;

					primary.emplace_back(MGstats, AssetLoader::getLoader().getModel("gunpod"), AssetLoader::getLoader().getModel("bullet"), glm::vec3(-0.0, -0.5, 1.0), glm::vec3(0.5), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)));
					entity.assign<Equipment>(primary, weapons);
					entity.assign<PointComponent>(100);
					enemyList.push_back(entity);

					count++;
				}
				std::cout << mi.missiontext << "\n";
				mTime = mi.time;
				timer.restart();
				active = true;
			}
		}

	}
};