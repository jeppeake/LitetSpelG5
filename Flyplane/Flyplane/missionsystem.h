#pragma once
#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "mission.h"
#include "playingstate.h"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
using namespace entityx;

struct MissionSystem : public entityx::System<MissionSystem> {
	bool active = false;
	std::vector<Mission> missions;
	unsigned int iActive = 0;
	PlayingState* state;
	Timer timer;
	double downtime = 2;
	double mTime = 10;
	Entity target;
	Entity formLeader;

	MissionSystem(PlayingState *state) : state(state) {
		std::string path = "assets/Presets/Missions";

		for (auto & p : fs::directory_iterator(path)) {
			std::string curPath = p.path().string();
			std::cout << "Loading mission: " << curPath << "\n";
			Mission m;
			m.loadMission(curPath);
			missions.push_back(m);
		}
	}
	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {
		if (active) {
			if (timer.elapsed() >= mTime) {
				std::cout << "Mission failed \n";
				timer.restart();
				active = false;
			}
		}
		else {
			if (timer.elapsed() >= downtime) {
				//spawn mission entities
				int i = rand() % missions.size();
				Mission mi = missions[i];

				//spawn houses
				for (HouseInfo house : missions[i].houses) {
					auto entity = es.create();
					entity.assign<ModelComponent>(house.model);
					entity.assign<Transform>(glm::vec3(house.pos.x, AssetLoader::getLoader().getHeightmap("testmap")->heightAt(glm::vec3(house.pos.x,0.f,house.pos.z))+10,house.pos.z));
					entity.assign<HealthComponent>(100);
					
					if (house.condition == CONDITION_DEFEND) {
						entity.assign<Target>(FACTION_PLAYER, 100);
						target = entity;
					}
					else if (house.condition == CONDITION_DESTROY)
						entity.assign<Target>(FACTION_AI, 100);

					entity.assign<CollisionComponent>();
				}

				//spawn leader
				if (mi.formation) {
					EnemyInfo enemy = mi.enemies[mi.iLeader];
					auto entity = es.create();
					float x = enemy.pos.x;
					float z = enemy.pos.z;

					glm::vec3 pos(x, AssetLoader::getLoader().getHeightmap("testmap")->heightAt(glm::vec3(x, 0, z)) + 1500, z);
					glm::quat orien(rand() % 100, rand() % 100, rand() % 100, rand() % 100);
					entity.assign<Transform>(pos, normalize(orien));
					entity.assign<Physics>(1000.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
					entity.assign<ModelComponent>(AssetLoader::getLoader().getModel("MIG-212A"));
					entity.assign<FlightComponent>(100.f, 200.f, 50.f, 1.5f, 0.5f);
					entity.assign<Target>(10.0, FACTION_AI);
					entity.assign<HealthComponent>(100.0);
					auto handle = entity.assign<ParticleComponent>();
					events.emit<AddParticleEvent>(TRAIL, handle);
					events.emit<AddParticleEvent>(ENGINE_TRAIL, handle);
					std::vector<Behaviour*> behaviours;

					entity.assign<FormationComponent>(100);

					std::vector<glm::vec3> plotter;
					plotter.push_back(glm::vec3(2500, 4500, 0));
					plotter.push_back(glm::vec3(2500, 4500, 2500));
					plotter.push_back(glm::vec3(0, 4500, 2500));
					plotter.push_back(glm::vec3(0, 4500, 0));

					//behaviours.push_back(new Constant_Turn(0));

					//behaviours.push_back(new Follow_Path(1, new Always_True(), plotter, true));
					//behaviours.push_back(new Hunt_Target(2, new Enemy_Close(5000.f), entity_p, 0.05f, 500.f));
					behaviours.push_back(new Follow_Target(7,new Always_True(), target));
					//behaviours.push_back(new Fly_Up(10, new Ground_Close_Front(4.f, 10)));
					behaviours.push_back(new Avoid_Closest(9, new Entity_Close(40.f)));

					entity.assign<AIComponent>(behaviours, true, true, false);
					entity.assign<CollisionComponent>();
					entity.assign<SoundComponent>(*AssetLoader::getLoader().getSoundBuffer("takeoff"));
					entity.assign<BurstSoundComponent>(*AssetLoader::getLoader().getSoundBuffer("machinegun"));

					WeaponStats MGstats = WeaponStats(10000, 3, 35000, 0.2, 0.02f, true);
					WeaponStats rocketpodstat = WeaponStats(14, 100, 700, 0.2, 0.5f, false);
					std::vector<Weapon> primary;
					std::vector<Weapon> secondary;
					secondary.emplace_back(rocketpodstat, AssetLoader::getLoader().getModel("rocketpod"), AssetLoader::getLoader().getModel("stinger"), glm::vec3(-0.9, -0.37, -1.5), glm::vec3(0.2), glm::vec3(0.8f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);
					secondary.emplace_back(rocketpodstat, AssetLoader::getLoader().getModel("rocketpod"), AssetLoader::getLoader().getModel("stinger"), glm::vec3(0.9, -0.37, -1.5), glm::vec3(0.2), glm::vec3(0.8f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);

					primary.emplace_back(MGstats, AssetLoader::getLoader().getModel("gunpod"), AssetLoader::getLoader().getModel("bullet"), glm::vec3(-0.0, -0.5, 1.0), glm::vec3(0.5), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)));
					entity.assign<Equipment>(primary, secondary);
					entity.assign<PointComponent>(100);
					formLeader = entity;
				}



				//spawn enemies

				int count = 0;
				for (EnemyInfo enemy : missions[i].enemies) {
					if (missions[i].iLeader == count) {
						count++;
						continue;
					}
						
					auto entity = es.create();
					float x = enemy.pos.x;
					float z = enemy.pos.z;

					glm::vec3 pos(x, AssetLoader::getLoader().getHeightmap("testmap")->heightAt(glm::vec3(x, 0, z)) + 1500, z);
					glm::quat orien(rand() % 100, rand() % 100, rand() % 100, rand() % 100);
					entity.assign<Transform>(pos, normalize(orien));
					entity.assign<Physics>(1000.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
					entity.assign<ModelComponent>(AssetLoader::getLoader().getModel("MIG-212A"));
					entity.assign<FlightComponent>(100.f, 200.f, 50.f, 1.5f, 0.5f);
					entity.assign<Target>(10.0, FACTION_AI);
					entity.assign<HealthComponent>(100.0);
					auto handle = entity.assign<ParticleComponent>();
					events.emit<AddParticleEvent>(TRAIL, handle);
					events.emit<AddParticleEvent>(ENGINE_TRAIL, handle);
					std::vector<Behaviour*> behaviours;

					std::vector<glm::vec3> plotter;
					plotter.push_back(glm::vec3(2500, 4500, 0));
					plotter.push_back(glm::vec3(2500, 4500, 2500));
					plotter.push_back(glm::vec3(0, 4500, 2500));
					plotter.push_back(glm::vec3(0, 4500, 0));

					//behaviours.push_back(new Constant_Turn(0));


					//behaviours.push_back(new Follow_Path(1, new Always_True(), plotter, true));
					behaviours.push_back(new Hunt_Target(5, new Enemy_Close(mi.huntPlayerDist), state->entity_p, 0.05f, mi.firingDistance));
					//behaviours.push_back(new Hunt_Target(3, new Always_True(), target, 0.05, 500.f));
					behaviours.push_back(new Fly_Up(10, new Ground_Close_Front(4.f, 10)));
					behaviours.push_back(new Avoid_Closest(9, new Entity_Close(40.f)));
					behaviours.push_back(new Form_On_Formation(6, new Always_True(), formLeader));

					entity.assign<AIComponent>(behaviours, true, true, false);
					entity.assign<CollisionComponent>();
					entity.assign<SoundComponent>(*AssetLoader::getLoader().getSoundBuffer("takeoff"));
					entity.assign<BurstSoundComponent>(*AssetLoader::getLoader().getSoundBuffer("machinegun"));

					WeaponStats MGstats = WeaponStats(10000, 3, 35000, 0.2, 0.02f, true);
					WeaponStats rocketpodstat = WeaponStats(14, 100, 700, 0.2, 0.5f, false);
					std::vector<Weapon> primary;
					std::vector<Weapon> secondary;
					secondary.emplace_back(rocketpodstat, AssetLoader::getLoader().getModel("rocketpod"), AssetLoader::getLoader().getModel("stinger"), glm::vec3(-0.9, -0.37, -1.5), glm::vec3(0.2), glm::vec3(0.8f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);
					secondary.emplace_back(rocketpodstat, AssetLoader::getLoader().getModel("rocketpod"), AssetLoader::getLoader().getModel("stinger"), glm::vec3(0.9, -0.37, -1.5), glm::vec3(0.2), glm::vec3(0.8f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);

					primary.emplace_back(MGstats, AssetLoader::getLoader().getModel("gunpod"), AssetLoader::getLoader().getModel("bullet"), glm::vec3(-0.0, -0.5, 1.0), glm::vec3(0.5), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)));
					entity.assign<Equipment>(primary, secondary);
					entity.assign<PointComponent>(100);


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