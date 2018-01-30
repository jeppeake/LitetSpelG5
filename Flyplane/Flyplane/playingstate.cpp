#include "playingstate.h"
#include <glm\gtc\constants.hpp>
#include <glm/gtx/quaternion.hpp>

#include "physicssystem.h"
#include "transform.h"
#include "weaponsystem.h"
#include "rendersystem.h"
#include "heightmap.h"
#include "playercomponent.h"
#include "playersystem.h"
#include "terraincomponent.h"
#include "flightsystem.h"
#include "flightcomponent.h"
#include "collisionsystem.h"
Model m;
Model projectile;
Model missile;
Model weaponmodel;
Model GAU;
Heightmap* hm;

entityx::Entity entity;
void PlayingState::init()
{
	m.load("assets/MIG-212A.fbx");
	/*
	* add systems
	* 
	* example

	ex.systems.add<System class here>();
	*/
	ex.systems.add<PhysicsSystem>();
	ex.systems.add<WeaponSystem>();
	ex.systems.add<RenderSystem>();
	ex.systems.add<PlayerSystem>();
	ex.systems.add<FlightSystem>();
	ex.systems.add<CollisionSystem>(hm);
	ex.systems.configure();

	/*
	* add entities
	*
	* example

	auto entity = ex.entities.create();
	entity.assign<Component class here>(Component constructor params);
	*/
	
	auto v = []() {
		return (rand() % 1000 - 500)*0.05;
	};


	for (int i = 0; i < 0; i++) {
		auto entity = ex.entities.create();
		glm::vec3 pos(rand() % 100, rand() % 100, rand() % 100);
		glm::quat orien(rand() % 100, rand() % 100, rand() % 100, rand() % 100);
		entity.assign<Transform>(pos, normalize(orien));
		entity.assign<Physics>(1000.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
		entity.assign <ModelComponent>(&m);
		entity.assign <FlightComponent>(1000.f, 2.f);
	}

	entity = ex.entities.create();
	glm::vec3 pos(rand() % 100, 1500, rand() % 100);
	glm::quat orien(1,0,0,0);
	entity.assign<Transform>(pos, normalize(orien));
	entity.assign<Physics>(1000.0, 1.0, glm::vec3(v(), v(), v()), glm::vec3(0.0, 0.0, 0.0));
	entity.assign <ModelComponent>(&m);
	entity.assign <PlayerComponent>();
	entity.assign <FlightComponent>(1000.f, 2.f);


	std::vector<Weapon> weapons;
	std::vector<Weapon> pweapons;
	
	projectile.load("assets/bullet.fbx");
	weaponmodel.load("assets/basicgun.fbx");
	GAU.load("assets/GAU-15.fbx");
	missile.load("assets/Weapons/Missiles/Fishrod/fishrod.fbx");

	WeaponStats stats = WeaponStats(1, 10, 1000, 0.2, 1.0f, false);
	WeaponStats stats2 = WeaponStats(10000, 10, 500, 0.2, 0.02f, true);
	WeaponStats bomb = WeaponStats(10, 1000000000, 0, 100, 0.5f, false);

	weapons.emplace_back(stats, &missile, &missile, glm::vec3(-1.7, -0.25, -1.5),glm::vec3(0.6), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, &missile, &missile, glm::vec3(1.7, -0.25, -1.5), glm::vec3(0.6), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	pweapons.emplace_back(stats2, &GAU, &projectile, glm::vec3(-0.3, 0.6, 1.8), glm::vec3(0.1), glm::angleAxis(180.f,glm::vec3(0,0,1)));
	weapons.emplace_back(bomb, &weaponmodel, &projectile, glm::vec3(0, -0.3, -0.1));


	hm = new Heightmap("assets/textures/cloude.png", "assets/textures/bog.png");
	entity.assign <Equipment>(pweapons, weapons);

	entityx::Entity terrain = ex.entities.create();
	terrain.assign<Terrain>(hm);
}

void PlayingState::update(double dt)
{
	double time = t.elapsed();

	/*double r = (1 + sin(time))*0.5;
	double g = (1 + sin(time + glm::two_pi<double>() / 3.0))*0.5;
	double b = (1 + sin(time + 2.0*glm::two_pi<double>() / 3.0))*0.5;*/
	
	glClearColor(100.0/255,149.0/255,234.0/255, 1.0);

	/*
	ex.systems.update<System class here>(dt);
	*/
	
	if(Input::isKeyDown(GLFW_KEY_SPACE))
		std::cout << ex.entities.size() << "\n";


	ex.systems.update<PlayerSystem>(dt);
	ex.systems.update<WeaponSystem>(dt);
	ex.systems.update<FlightSystem>(dt);
	ex.systems.update<PhysicsSystem>(dt);
	ex.systems.update<CollisionSystem>(dt);
	ex.systems.update<RenderSystem>(dt);
}
