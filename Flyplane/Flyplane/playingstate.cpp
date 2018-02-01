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
#include "soundsystem.h"

#include "aicomponent.h"
#include "aisystem.h"
#include "behaviour.h"
#include "constant_turn.h"
#include "soundbuffers.h"
#include "follow_path.h"

Model m;
Model projectile;
Model missile;
Model weaponmodel;
Model GAU;
Model gunpod;
Heightmap* hm;
sf::SoundBuffer flyingSB;
sf::SoundBuffer missileSB;
sf::SoundBuffer bulletSB;
sf::SoundBuffer machinegunSB;

entityx::Entity entity;

void PlayingState::init()
{
	if (!flyingSB.loadFromFile("assets/Sound/airplane-takeoff.wav"))
		std::cout << "sound coludnt load" << std::endl;
	if (!missileSB.loadFromFile("assets/Sound/Missle_Launch.wav"))
		std::cout << "sound coludnt load" << std::endl;
	if (!bulletSB.loadFromFile("assets/Sound/Sniper_Rifle_short.wav"))
		std::cout << "sound coludnt load" << std::endl;
	if (!machinegunSB.loadFromFile("assets/Sound/Machine_gun.wav"))
		std::cout << "sound coludnt load" << std::endl;
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
	ex.systems.add<AISystem>();
	ex.systems.add<SoundSystem>();
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


	for (int i = 0; i < 1; i++) {
		auto entity = ex.entities.create();
		glm::vec3 pos(rand() % 100, 2500, rand() % 100);
		glm::quat orien(rand() % 100, rand() % 100, rand() % 100, rand() % 100);
		entity.assign<Transform>(pos, normalize(orien));
		entity.assign<Physics>(1000.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
		entity.assign <ModelComponent>(&m);
		entity.assign <FlightComponent>(1000.f, 1.f);
		std::vector<Behaviour*> behaviours;

		std::vector<glm::vec3> plotter;
		plotter.push_back(glm::vec3(500, 2500, 0));
		plotter.push_back(glm::vec3(500, 2500, 500));
		plotter.push_back(glm::vec3(0, 2500, 500));
		plotter.push_back(glm::vec3(0, 2500, 0));

		behaviours.push_back(new Constant_Turn(0));
		behaviours.push_back(new Follow_Path(1, plotter, true));

		entity.assign<AIComponent>(behaviours);
		entity.assign<CollisionComponent>();
		entity.assign<SoundComponent>(flyingSB, true);
	}

	//entity = ex.entities.create();
	//entity.assign<SoundComponent>(soundBuffer);

	entity = ex.entities.create();
	float x = 500;
	float z = 500;
	glm::vec3 pos(x, 2500, z);
	glm::quat orien(1,0,0,0);
	entity.assign<Transform>(pos, normalize(orien));
	entity.assign<Physics>(1000.0, 1.0, glm::vec3(v(), v(), v()), glm::vec3(0.0, 0.0, 0.0));
	entity.assign <ModelComponent>(&m);
	entity.assign <PlayerComponent>();
	entity.assign <FlightComponent>(1000.f, 2.f);
	entity.assign <CollisionComponent>();
	entity.assign<SoundComponent>(flyingSB, true);

	std::vector<Weapon> weapons;
	std::vector<Weapon> pweapons;
	
	projectile.load("assets/bullet.fbx");
	weaponmodel.load("assets/basicgun.fbx");
	GAU.load("assets/GAU-15.fbx");
	gunpod.load("assets/Weapons/Guns/37mm_gunpod/37mm_gunpod.fbx");
	missile.load("assets/Weapons/Missiles/Fishrod/fishrod.fbx");

	WeaponStats stats = WeaponStats(1, 1000, 1000, 0.2, 1.0f, false);
	WeaponStats stats2 = WeaponStats(10000, 10, 500, 0.2, 0.02f, true);
	WeaponStats bomb = WeaponStats(10, 1000000000, 0, 100, 0.5f, true);

	weapons.emplace_back(stats, &missile, &missile, glm::vec3(-0.9, -0.25, -1.5), glm::vec3(0.6), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, &missile, &missile, glm::vec3(0.9, -0.25, -1.5), glm::vec3(0.6), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, &missile, &missile, glm::vec3(-1.3, -0.25, -1.5), glm::vec3(0.6), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, &missile, &missile, glm::vec3(1.3, -0.25, -1.5), glm::vec3(0.6), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, &missile, &missile, glm::vec3(-1.7, -0.25, -1.5),glm::vec3(0.6), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, &missile, &missile, glm::vec3(1.7, -0.25, -1.5), glm::vec3(0.6), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, &missile, &missile, glm::vec3(-2.1, -0.25, -1.5), glm::vec3(0.6), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, &missile, &missile, glm::vec3(2.1, -0.25, -1.5), glm::vec3(0.6), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, &missile, &missile, glm::vec3(-2.5, -0.25, -1.5), glm::vec3(0.6), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, &missile, &missile, glm::vec3(2.5, -0.25, -1.5), glm::vec3(0.6), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	pweapons.emplace_back(stats2, &gunpod, &projectile, glm::vec3(-0.0, -0.5, 1.0), glm::vec3(0.5), glm::angleAxis(0.f,glm::vec3(0,0,1)));
	weapons.emplace_back(bomb, &weaponmodel, &missile, glm::vec3(0, -0.3, -0.1));


	hm = new Heightmap("assets/textures/slojp.png", "assets/textures/bog.png");
	hm->pos.x -= 2560;
	hm->pos.z -= 2560;
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
	ex.systems.update<AISystem>(dt);
	ex.systems.update<WeaponSystem>(dt);
	ex.systems.update<FlightSystem>(dt);
	ex.systems.update<PhysicsSystem>(dt);
	ex.systems.update<CollisionSystem>(dt);
	ex.systems.update<RenderSystem>(dt);
	ex.systems.update<SoundSystem>(dt);

	if (Input::isKeyPressed(GLFW_KEY_F5)) {
		this->changeState(new PlayingState());
	}
}
