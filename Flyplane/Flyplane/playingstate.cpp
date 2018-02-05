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
#include "gameoversystem.h"

#include "aicomponent.h"
#include "aisystem.h"
#include "behaviour.h"
#include "constant_turn.h"
#include "soundbuffers.h"
#include "follow_path.h"
#include "targetcomponent.h"



entityx::Entity entity;

sf::SoundBuffer* missileSB;

void PlayingState::init() 
{

	sf::SoundBuffer* flyingSB;
	sf::SoundBuffer* bulletSB;
	sf::SoundBuffer* machinegunSB;

	/*if (!flyingSB.loadFromFile("assets/Sound/airplane-takeoff.wav"))
		std::cout << "sound coludnt load" << std::endl;
	if (!missileSB.loadFromFile("assets/Sound/Missle_Launch.wav"))
		std::cout << "sound coludnt load" << std::endl;
	if (!bulletSB.loadFromFile("assets/Sound/Sniper_Rifle_short.wav"))
		std::cout << "sound coludnt load" << std::endl;
	if (!machinegunSB.loadFromFile("assets/Sound/Machine_gun.wav"))
		std::cout << "sound coludnt load" << std::endl;*/

	//load all assets, all assets are given a reference name to used when retreiving it
	assetLoader.loadModel("assets/bullet.fbx", "bullet");
	assetLoader.loadModel("assets/basicgun.fbx", "basicgun");
	assetLoader.loadModel("assets/GAU-15.fbx", "GAU-15");
	assetLoader.loadModel("assets/Weapons/Guns/37mm_gunpod/37mm_gunpod.fbx", "gunpod");
	assetLoader.loadModel("assets/Weapons/Missiles/Fishrod/fishrod.fbx", "fishrod");
	assetLoader.loadModel("assets/Weapons/Rocketpod/rocketpod.fbx", "rocketpod");
	assetLoader.loadModel("assets/Weapons/Missiles/Stinger/stinger.fbx", "stinger");
	assetLoader.loadModel("assets/MIG-212A.fbx", "MIG-212A");
	assetLoader.loadModel("assets/Weapons/missiles/ALAAT-10/ALAAT-10.fbx", "ALAAT-10");

	assetLoader.loadHeightmap("assets/textures/slojp.png", "assets/textures/grass.png", "testmap");

	assetLoader.loadSound("assets/Sound/airplane-takeoff.wav", "takeoff");
	assetLoader.loadSound("assets/Sound/Missle_Launch.wav", "missile");
	assetLoader.loadSound("assets/Sound/Sniper_Rifle_short.wav", "sniperrifle");
	assetLoader.loadSound("assets/Sound/Machine_gun.wav", "machinegun");


	//get all assets (not really needed, can be used inline)
	flyingSB = assetLoader.getSoundBuffer("takeoff");
	missileSB = assetLoader.getSoundBuffer("missile");
	bulletSB = assetLoader.getSoundBuffer("sniperrifle");
	machinegunSB = assetLoader.getSoundBuffer("machinegun");

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
	ex.systems.add<CollisionSystem>(assetLoader.getHeightmap("testmap"));
	ex.systems.add<AISystem>();
	ex.systems.add<SoundSystem>();
	ex.systems.add<GameOver>();
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
		entity.assign <ModelComponent>(assetLoader.getModel("MIG-212A"));
		entity.assign <FlightComponent>(300.f, 1.f);
		entity.assign<Target>(10.0, FACTION_AI);
		std::vector<Behaviour*> behaviours;

		std::vector<glm::vec3> plotter;
		plotter.push_back(glm::vec3(2500, 2500, 0));
		plotter.push_back(glm::vec3(2500, 2500, 2500));
		plotter.push_back(glm::vec3(0, 2500, 2500));
		plotter.push_back(glm::vec3(0, 2500, 0));

		behaviours.push_back(new Constant_Turn(0));
		behaviours.push_back(new Follow_Path(1, plotter, true));

		entity.assign<AIComponent>(behaviours);
		entity.assign<CollisionComponent>();
		entity.assign<SoundComponent>(*flyingSB);
	}

	//entity = ex.entities.create();
	//entity.assign<SoundComponent>(soundBuffer);
	
	// ---	PLAYER	---
	entity = ex.entities.create();
	float x = 500;
	float z = 500;
	glm::vec3 pos(x, 2500, z);
	glm::quat orien(1,0,0,0);
	entity.assign<Transform>(pos, normalize(orien));
	entity.assign<Physics>(1000.0, 1.0, glm::vec3(v(), v(), v()), glm::vec3(0.0, 0.0, 0.0));
	entity.assign <ModelComponent>(assetLoader.getModel("MIG-212A"));
	entity.assign <PlayerComponent>();
	entity.assign <FlightComponent>(200.f, 2.f);
	entity.assign <CollisionComponent>();
	entity.assign<SoundComponent>(*flyingSB);
	entity.assign<BurstSoundComponent>(*machinegunSB);
	entity.assign<Target>(10.0, FACTION_PLAYER);

	std::vector<Weapon> weapons;
	std::vector<Weapon> pweapons;

	



	WeaponStats stats = WeaponStats(1, 1000, 400, 0.2, 1.0f, false, 2.f);
	WeaponStats rocketpodstat = WeaponStats(14, 100, 700, 0.2, 0.5f, false);
	WeaponStats stats2 = WeaponStats(10000, 3, 500, 0.2, 0.02f, true);
	WeaponStats bomb = WeaponStats(10, 1000000000, 0, 100, 0.5f, true);

	weapons.emplace_back(rocketpodstat, assetLoader.getModel("rocketpod"), assetLoader.getModel("stinger"), glm::vec3(-0.9, -0.37, -1.5), glm::vec3(0.2), glm::vec3(0.8f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);
	weapons.emplace_back(rocketpodstat, assetLoader.getModel("rocketpod"), assetLoader.getModel("stinger"), glm::vec3(0.9, -0.37, -1.5), glm::vec3(0.2), glm::vec3(0.8f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);
	weapons.emplace_back(stats, assetLoader.getModel("fishrod"), assetLoader.getModel("fishrod"), glm::vec3(-1.3, -0.25, -1.5), glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, assetLoader.getModel("fishrod"), assetLoader.getModel("fishrod"), glm::vec3(1.3, -0.25, -1.5), glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, assetLoader.getModel("fishrod"), assetLoader.getModel("fishrod"), glm::vec3(-1.7, -0.25, -1.5),glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, assetLoader.getModel("fishrod"), assetLoader.getModel("fishrod"), glm::vec3(1.7, -0.25, -1.5), glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, assetLoader.getModel("fishrod"), assetLoader.getModel("fishrod"), glm::vec3(-2.1, -0.25, -1.5), glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, assetLoader.getModel("fishrod"), assetLoader.getModel("fishrod"), glm::vec3(2.1, -0.25, -1.5), glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, assetLoader.getModel("fishrod"), assetLoader.getModel("fishrod"), glm::vec3(-2.5, -0.25, -1.5), glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, assetLoader.getModel("fishrod"), assetLoader.getModel("fishrod"), glm::vec3(2.5, -0.25, -1.5), glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	pweapons.emplace_back(stats2, assetLoader.getModel("gunpod"), assetLoader.getModel("bullet"), glm::vec3(-0.0, -0.5, 1.0), glm::vec3(0.5), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(0.f,glm::vec3(0,0,1)));
	weapons.emplace_back(bomb, assetLoader.getModel("bullet"), assetLoader.getModel("fishrod"), glm::vec3(0, -0.3, -0.1));


	assetLoader.getHeightmap("testmap")->pos.x -= 2560;
	assetLoader.getHeightmap("testmap")->pos.z -= 2560;
	entity.assign <Equipment>(pweapons, weapons);

	entityx::Entity terrain = ex.entities.create();
	terrain.assign<Terrain>(assetLoader.getHeightmap("testmap"));
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
