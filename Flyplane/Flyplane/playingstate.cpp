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
#include "condition.h"
#include "always_true.h"
#include "enemy_close.h"
#include "follow_player.h"
#include "ground_close_front.h"
#include "fly_up.h"
#include "follow_target.h"
#include "window.h"
#include "menustate.h"

#include "menustate.h"
#include "pointcomponent.h"
#include <string>
#include "window.h"
#include "highscore.h"
#include "backtomenuaction.h"
#include "restartaction.h"

entityx::Entity entity;
entityx::Entity entity2;

sf::SoundBuffer* missileSB;

void PlayingState::spawnEnemies(int nr) {

	for (int i = 0; i < nr; i++) {
		auto entity = ex.entities.create();
		glm::vec3 pos(rand() % 100, rand() % 300 + 2000, rand() % 100);
		glm::quat orien(rand() % 100, rand() % 100, rand() % 100, rand() % 100);
		entity.assign<Transform>(pos, normalize(orien));
		entity.assign<Physics>(1000.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
		entity.assign <ModelComponent>(AssetLoader::getLoader().getModel("MIG-212A"));
		entity.assign <FlightComponent>(200.f, 1.f);
		entity.assign<Target>(10.0, FACTION_AI);
		std::vector<Behaviour*> behaviours;

		std::vector<glm::vec3> plotter;
		plotter.push_back(glm::vec3(2500, 2500, 0));
		plotter.push_back(glm::vec3(2500, 2500, 2500));
		plotter.push_back(glm::vec3(0, 2500, 2500));
		plotter.push_back(glm::vec3(0, 2500, 0));

		//behaviours.push_back(new Constant_Turn(0));
		behaviours.push_back(new Follow_Path(1, new Always_True(), plotter, true));
		behaviours.push_back(new Follow_Player(2, new Enemy_Close(2000.f)));

		entity.assign<AIComponent>(behaviours);
		entity.assign<CollisionComponent>();
		entity.assign<SoundComponent>(*flyingSB);
		entity.assign<PointComponent>(100);

		//std::cout << "Enemy added\n";
	}
}

void PlayingState::drawHighscore() {
	glm::vec2 pos;
	pos.x = 800;
	pos.y = 300;
	AssetLoader::getLoader().getMenutext()->drawText("HIGH SCORES", pos, glm::vec3(1, 0, 0), 0.8);
}

void PlayingState::startMenu() {
	this->changeState(new MenuState());
}

void PlayingState::restart() {
	this->changeState(new PlayingState());
}


void PlayingState::init() 
{

	Window::getWindow().showCursor(true);

	bHandler.addButton(new Button("Restart", glm::vec2(100, 100), glm::vec2(120, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new RestartAction(this)));
	bHandler.addButton(new Button("Back to menu", glm::vec2(100, 150), glm::vec2(200, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new BackToMenuAction(this)));


	/*sf::SoundBuffer* flyingSB;
	sf::SoundBuffer* bulletSB;
	sf::SoundBuffer* machinegunSB;*/

	//load all assets, all assets are given a reference name to used when retreiving it
	AssetLoader::getLoader().loadModel("assets/bullet.fbx", "bullet");
	AssetLoader::getLoader().loadModel("assets/basicgun.fbx", "basicgun");
	AssetLoader::getLoader().loadModel("assets/GAU-15.fbx", "GAU-15");
	AssetLoader::getLoader().loadModel("assets/Weapons/Guns/37mm_gunpod/37mm_gunpod.fbx", "gunpod");
	AssetLoader::getLoader().loadModel("assets/Weapons/Missiles/Fishrod/fishrod.fbx", "fishrod");
	AssetLoader::getLoader().loadModel("assets/Weapons/Rocketpod/rocketpod.fbx", "rocketpod");
	AssetLoader::getLoader().loadModel("assets/Weapons/Missiles/Stinger/stinger.fbx", "stinger");
	AssetLoader::getLoader().loadModel("assets/MIG-212A.fbx", "MIG-212A");
	AssetLoader::getLoader().loadModel("assets/Weapons/missiles/ALAAT-10/ALAAT-10.fbx", "ALAAT-10");

	AssetLoader::getLoader().loadHeightmap("assets/textures/slojp.png", "assets/textures/grass.png", "testmap");

	AssetLoader::getLoader().loadSound("assets/Sound/airplane-takeoff.wav", "takeoff");
	AssetLoader::getLoader().loadSound("assets/Sound/Missle_Launch.wav", "missile");
	AssetLoader::getLoader().loadSound("assets/Sound/Sniper_Rifle_short.wav", "sniperrifle");
	AssetLoader::getLoader().loadSound("assets/Sound/Machine_gun.wav", "machinegun");


	//get all assets (not really needed, can be used inline)
	flyingSB = AssetLoader::getLoader().getSoundBuffer("takeoff");
	missileSB = AssetLoader::getLoader().getSoundBuffer("missile");
	bulletSB = AssetLoader::getLoader().getSoundBuffer("sniperrifle");
	machinegunSB = AssetLoader::getLoader().getSoundBuffer("machinegun");

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
	ex.systems.add<CollisionSystem>(AssetLoader::getLoader().getHeightmap("testmap"), this);
	ex.systems.add<AISystem>();
	ex.systems.add<SoundSystem>();
	ex.systems.add<GameOver>(this);
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

	entity2 = ex.entities.create();
	float x = 400;
	float z = 500;
	glm::vec3 pos(x, 4500, z);
	glm::quat orien(1, 0, 0, 0);
	entity2.assign<Transform>(pos, normalize(orien));
	entity2.assign<Physics>(1000.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
	entity2.assign <ModelComponent>(AssetLoader::getLoader().getModel("MIG-212A"));
	entity2.assign <FlightComponent>(200.f, 2.f);
	entity2.assign <CollisionComponent>();
	std::vector<Behaviour*> behaviours;

	std::vector<glm::vec3> plotter;
	plotter.push_back(glm::vec3(2500, 2500, 0));
	plotter.push_back(glm::vec3(2500, 2500, 2500));
	plotter.push_back(glm::vec3(0, 2500, 2500));
	plotter.push_back(glm::vec3(0, 2500, 0));

	//spawnEnemies(20);
	//behaviours.push_back(new Constant_Turn(0));
	behaviours.push_back(new Follow_Path(1, new Always_True(), plotter, true));

	entity2.assign<AIComponent>(behaviours);
	entity2.assign<Target>(10.0, FACTION_DUMMY);

	int enemies = 0;
	for (int i = 0; i < 1; i++) {
		auto entity = ex.entities.create();
		glm::vec3 pos(rand() % 100, 2500, rand() % 100);
		glm::quat orien(rand() % 100, rand() % 100, rand() % 100, rand() % 100);
		entity.assign<Transform>(pos, normalize(orien));
		entity.assign<Physics>(1000.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
		entity.assign <ModelComponent>(AssetLoader::getLoader().getModel("MIG-212A"));
		entity.assign <FlightComponent>(200.f, 2.f);
		entity.assign<Target>(10.0, FACTION_AI);
		std::vector<Behaviour*> behaviours;

		std::vector<glm::vec3> plotter;
		plotter.push_back(glm::vec3(2500, 2500, 0));
		plotter.push_back(glm::vec3(2500, 2500, 2500));
		plotter.push_back(glm::vec3(0, 2500, 2500));
		plotter.push_back(glm::vec3(0, 2500, 0));

		//behaviours.push_back(new Constant_Turn(0));
		behaviours.push_back(new Follow_Path(1, new Always_True(), plotter, true));
		behaviours.push_back(new Follow_Player(2, new Enemy_Close(2000.f)));
		//behaviours.push_back(new Follow_Target(9, new Always_True(), entity2));
		behaviours.push_back(new Fly_Up(10, new Ground_Close_Front(4.f, 10)));

		entity.assign<AIComponent>(behaviours);
		entity.assign<CollisionComponent>();
		entity.assign<SoundComponent>(*flyingSB);
		enemies++;
		//std::cout << "Enemy added\n";
	}
	std::cout << "\nAdded " << enemies << " enemies.\n";

	//entity = ex.entities.create();
	//entity.assign<SoundComponent>(soundBuffer);
	
	// ---	PLAYER	---
	entity = ex.entities.create();
	x = 500;
	z = 500;
	//glm::vec3 pos(x, 2500, z);
	//glm::quat orien(1,0,0,0);
	entity.assign<Transform>(pos, normalize(orien));
	entity.assign<Physics>(1000.0, 1.0, glm::vec3(v(), v(), v()), glm::vec3(0.0, 0.0, 0.0));
	entity.assign <ModelComponent>(AssetLoader::getLoader().getModel("MIG-212A"));
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

	weapons.emplace_back(rocketpodstat, AssetLoader::getLoader().getModel("rocketpod"), AssetLoader::getLoader().getModel("stinger"), glm::vec3(-0.9, -0.37, -1.5), glm::vec3(0.2), glm::vec3(0.8f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);
	weapons.emplace_back(rocketpodstat, AssetLoader::getLoader().getModel("rocketpod"), AssetLoader::getLoader().getModel("stinger"), glm::vec3(0.9, -0.37, -1.5), glm::vec3(0.2), glm::vec3(0.8f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);
	weapons.emplace_back(stats, AssetLoader::getLoader().getModel("fishrod"), AssetLoader::getLoader().getModel("fishrod"), glm::vec3(-1.3, -0.25, -1.5), glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, AssetLoader::getLoader().getModel("fishrod"), AssetLoader::getLoader().getModel("fishrod"), glm::vec3(1.3, -0.25, -1.5), glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, AssetLoader::getLoader().getModel("fishrod"), AssetLoader::getLoader().getModel("fishrod"), glm::vec3(-1.7, -0.25, -1.5),glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, AssetLoader::getLoader().getModel("fishrod"), AssetLoader::getLoader().getModel("fishrod"), glm::vec3(1.7, -0.25, -1.5), glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, AssetLoader::getLoader().getModel("fishrod"), AssetLoader::getLoader().getModel("fishrod"), glm::vec3(-2.1, -0.25, -1.5), glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, AssetLoader::getLoader().getModel("fishrod"), AssetLoader::getLoader().getModel("fishrod"), glm::vec3(2.1, -0.25, -1.5), glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, AssetLoader::getLoader().getModel("fishrod"), AssetLoader::getLoader().getModel("fishrod"), glm::vec3(-2.5, -0.25, -1.5), glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	weapons.emplace_back(stats, AssetLoader::getLoader().getModel("fishrod"), AssetLoader::getLoader().getModel("fishrod"), glm::vec3(2.5, -0.25, -1.5), glm::vec3(0.6), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(180.f, glm::vec3(0, 0, 1)), true, true);
	pweapons.emplace_back(stats2, AssetLoader::getLoader().getModel("gunpod"), AssetLoader::getLoader().getModel("bullet"), glm::vec3(-0.0, -0.5, 1.0), glm::vec3(0.5), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(0.f,glm::vec3(0,0,1)));
	weapons.emplace_back(bomb, AssetLoader::getLoader().getModel("bullet"), AssetLoader::getLoader().getModel("fishrod"), glm::vec3(0, -0.3, -0.1));


	entity.assign <Equipment>(pweapons, weapons);

	entityx::Entity terrain = ex.entities.create();
	terrain.assign<Terrain>(AssetLoader::getLoader().getHeightmap("testmap"));
}

void PlayingState::update(double dt)
{
	if (deltatime.elapsed() > 30) {
		deltatime.restart();
		spawnEnemies(5);
	}

	glClearColor(100.0/255,149.0/255,234.0/255, 1.0);

	/*
	ex.systems.update<System class here>(dt);
	*/
	
	if (Input::isKeyPressed(GLFW_KEY_ESCAPE))
		this->menuOpen = !this->menuOpen;
	
	
	if(Input::isKeyDown(GLFW_KEY_SPACE))
		std::cout << ex.entities.size() << "\n";


	

	/*bool playerAlive = false;

	ComponentHandle<PlayerComponent> p_player;
	for (entityx::Entity entity : ex.entities.entities_with_components(p_player)) {
		playerAlive = true;
	}*/

	

	if (playerAlive && !menuOpen) {
		points += 10 * dt;
		Window::getWindow().showCursor(false);
		ex.systems.update<PlayerSystem>(dt);
		ex.systems.update<AISystem>(dt);
		ex.systems.update<WeaponSystem>(dt);
		ex.systems.update<FlightSystem>(dt);
		ex.systems.update<PhysicsSystem>(dt);
		ex.systems.update<CollisionSystem>(dt);
		ex.systems.update<SoundSystem>(dt);
		ex.systems.update<RenderSystem>(dt);
	}
	else {
		ex.systems.update<RenderSystem>(dt);
		if (!playerAlive) {
			AssetLoader::getLoader().getMenutext()->drawText("WASTED", glm::vec2(500, 500), glm::vec3(1, 0, 0), 3);
			drawHighscore();
		}
		Window::getWindow().showCursor(true);
		bHandler.drawButtons();
		bHandler.handleButtonClicks();
	}
	
	

	glm::vec2 pos = glm::vec2(10, Window::getWindow().size().y - 20);
	AssetLoader::getLoader().getText()->drawText("Score: " + std::to_string(int(points)), pos, glm::vec3(1, 0, 0), 0.4);

	if (Input::isKeyPressed(GLFW_KEY_F5)) {
		this->changeState(new PlayingState());
	}
}

void PlayingState::gameOver() {
	playerAlive = false;
	highscore.addScore(name, points);
	/*Highscore list;
	glm::vec2 pos = Window::getWindow().size();
	pos.x = pos.x / 2 - 20;
	pos.y -= 50;
	AssetLoader::getLoader().getText()->drawText("HIGH SCORES", pos, glm::vec3(1, 0, 0), 0.4);*/
	//this->changeState(new GameOverState(name, points));
}
