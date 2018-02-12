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
#include "entity_close.h"
#include "avoid_closest.h"
#include "hunt_target.h"
#include "hunt_player.h"
#include "form_on_formation.h"

#include "menustate.h"
#include "pointcomponent.h"
#include <string>
#include "window.h"
#include "highscore.h"
#include "backtomenuaction.h"
#include "restartaction.h"

#include "weaponpreset.h"
#include "planepreset.h"
#include <fstream>

//entityx::Entity entity;
entityx::Entity entity_p;
entityx::Entity entity_formation;
entityx::Entity entity2;

sf::SoundBuffer* missileSB;

void PlayingState::spawnEnemies(int nr) {

	for (int i = 0; i < nr; i++) {
		auto entity = ex.entities.create();
		glm::vec3 pos(rand() % 2000,rand() % 1000 + 1500, rand() % 2000);
		glm::quat orien(rand() % 100, rand() % 100, rand() % 100, rand() % 100);
		entity.assign<Transform>(pos, normalize(orien));
		entity.assign<Physics>(1000.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
		entity.assign <ModelComponent>(AssetLoader::getLoader().getModel("MIG-212A"));
		entity.assign <FlightComponent>(200.f, 1.5f);
		entity.assign<Target>(10.0, FACTION_AI);
		std::vector<Behaviour*> behaviours;

		std::vector<glm::vec3> plotter;
		plotter.push_back(glm::vec3(2500, 2500, 0));
		plotter.push_back(glm::vec3(2500, 2500, 2500));
		plotter.push_back(glm::vec3(0, 2500, 2500));
		plotter.push_back(glm::vec3(0, 2500, 0));

		//behaviours.push_back(new Constant_Turn(0));
		behaviours.push_back(new Follow_Path(1, new Always_True(), plotter, true));
		behaviours.push_back(new Hunt_Target(2, new Enemy_Close(5000.f), entity_p, 0.05f, 500.f));
		behaviours.push_back(new Hunt_Target(3, new Always_True(), entity2, 0.05, 500.f));
		behaviours.push_back(new Fly_Up(10, new Ground_Close_Front(4.f, 10)));
		behaviours.push_back(new Avoid_Closest(9, new Entity_Close(20.f)));
		behaviours.push_back(new Form_On_Formation(8, new Always_True(), entity_formation));

		entity.assign<AIComponent>(behaviours, true, true);
		entity.assign<CollisionComponent>();
		entity.assign<SoundComponent>(*flyingSB);
		entity.assign<BurstSoundComponent>(*machinegunSB);

		WeaponStats MGstats = WeaponStats(10000, 3, 500, 0.2, 0.02f, true);
		WeaponStats rocketpodstat = WeaponStats(14, 100, 700, 0.2, 0.5f, false);
		std::vector<Weapon> primary;
		std::vector<Weapon> secondary;
		secondary.emplace_back(rocketpodstat, AssetLoader::getLoader().getModel("rocketpod"), AssetLoader::getLoader().getModel("stinger"), glm::vec3(-0.9, -0.37, -1.5), glm::vec3(0.2), glm::vec3(0.8f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);
		secondary.emplace_back(rocketpodstat, AssetLoader::getLoader().getModel("rocketpod"), AssetLoader::getLoader().getModel("stinger"), glm::vec3(0.9, -0.37, -1.5), glm::vec3(0.2), glm::vec3(0.8f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);

		primary.emplace_back(MGstats, AssetLoader::getLoader().getModel("gunpod"), AssetLoader::getLoader().getModel("bullet"), glm::vec3(-0.0, -0.5, 1.0), glm::vec3(0.5), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)));
		entity.assign<Equipment>(primary, secondary);
		entity.assign<PointComponent>(100);
	}
}

void PlayingState::drawHighscore() {
	glm::vec2 pos;
	pos.x = 800;
	pos.y = 400;
	string* p = Highscore::getHighscore().getHighscoreList();
	AssetLoader::getLoader().getHighscoreText()->drawText("HIGH SCORES", pos, glm::vec3(1, 1, 1), 0.8);
	pos.x = 700;
	for (int i = 0; i < 5; i++) {
		pos.y -= 40;
		AssetLoader::getLoader().getHighscoreText()->drawText(p[i], pos, glm::vec3(1, 1, 1), 0.7);
	}
}

auto v = []() {
	return (rand() % 1000 - 500)*0.05;
};

void PlayingState::loadLoadout()
{
	std::ifstream file("loadout.txt");
	std::string str;

	//read name
	PlanePreset pp;
	std::getline(file, str);
	pp.load(str);


	float x = 0;
	float z = 0;
	float y = AssetLoader::getLoader().getHeightmap("testmap")->heightAt(glm::vec3(x, 0, z)) + 1500;
	glm::vec3 pos(x, y, z);
	glm::quat orien(1, 0, 0, 0);
	// ---	PLAYER	---
	entity_p = ex.entities.create();
	x = 500;
	z = 500;
	//glm::vec3 pos(x, 2500, z);
	//glm::quat orien(1,0,0,0);

	AssetLoader::getLoader().loadModel(pp.model, pp.name);

	entity_p.assign<Transform>(pos, normalize(orien));
	entity_p.assign<Physics>(1000.0, 1.0, glm::vec3(v(), v(), v()), glm::vec3(0.0, 0.0, 0.0));
	entity_p.assign <ModelComponent>(AssetLoader::getLoader().getModel(pp.name));
	entity_p.assign <PlayerComponent>();
	entity_p.assign <FlightComponent>(pp.normalspeed, pp.turnrate);
	entity_p.assign <CollisionComponent>();
	entity_p.assign<SoundComponent>(*flyingSB);
	entity_p.assign<BurstSoundComponent>(*machinegunShortSB);
	entity_p.assign<Target>(10.0, FACTION_PLAYER);


	std::vector<Weapon> weapons;
	std::vector<Weapon> pweapons;


	for (int i = 0; i < pp.wepPos.size(); i++) {
		std::getline(file, str);
		if (str.compare("0") != 0) {
			WeaponPreset wp;
			wp.load(str);

			AssetLoader::getLoader().loadModel(wp.model, wp.name);
			AssetLoader::getLoader().loadModel(wp.projModel, wp.projModel);

			WeaponStats stats = WeaponStats(wp.ammo, wp.lifetime, wp.speed, wp.mass, wp.cooldown, wp.infAmmo, wp.turnRate);

			weapons.emplace_back(stats, AssetLoader::getLoader().getModel(wp.name), AssetLoader::getLoader().getModel(wp.projModel), pp.wepPos[i], glm::vec3(wp.scale), glm::vec3(wp.projScale), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), wp.isMissile, wp.dissappear);
		}
	}
	std::getline(file, str);
	entity_p.component<ModelComponent>().get()->mptr->texture = *AssetLoader::getLoader().getTexture(pp.textureNames[std::stoi(str)]);

	WeaponStats stats2 = WeaponStats(10000, 3, 500, 0.2, 0.02f, true);
	pweapons.emplace_back(stats2, AssetLoader::getLoader().getModel("gunpod"), AssetLoader::getLoader().getModel("bullet"), glm::vec3(-0.0, -0.25, 0.5), glm::vec3(0.25), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)));
	WeaponStats bomb = WeaponStats(10, 1000000000, 0, 100, 0.5f, true);
	weapons.emplace_back(bomb, AssetLoader::getLoader().getModel("bullet"), AssetLoader::getLoader().getModel("fishrod"), glm::vec3(0, -0.3, -0.1));

	entity_p.assign <Equipment>(pweapons, weapons);

	entityx::Entity terrain = ex.entities.create();

}

void PlayingState::startMenu() {
	this->changeState(new MenuState());
}

void PlayingState::restart() {
	this->changeState(new PlayingState(name));
}




void PlayingState::init() 
{

	Window::getWindow().showCursor(true);

	bHandler.addButton(new Button("Restart", glm::vec2(100, 100), glm::vec2(120, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new RestartAction(this), "buttonforward"));
	bHandler.addButton(new Button("Back to menu", glm::vec2(100, 150), glm::vec2(200, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new BackToMenuAction(this), "buttonback"));


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
	AssetLoader::getLoader().loadSound("assets/Sound/Machine_gun_short.wav", "machinegunShort");


	//get all assets (not really needed, can be used inline)
	flyingSB = AssetLoader::getLoader().getSoundBuffer("takeoff");
	missileSB = AssetLoader::getLoader().getSoundBuffer("missile");
	bulletSB = AssetLoader::getLoader().getSoundBuffer("sniperrifle");
	machinegunSB = AssetLoader::getLoader().getSoundBuffer("machinegun");
	machinegunShortSB = AssetLoader::getLoader().getSoundBuffer("machinegunShort");

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
	
	loadLoadout();

	entity2 = ex.entities.create();
	float x = 0;
	float z = 100;
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

	spawnEnemies(5);
	//behaviours.push_back(new Constant_Turn(0));
	behaviours.push_back(new Follow_Path(1, new Always_True(), plotter, true));

	entity2.assign<AIComponent>(behaviours, true, true);
	entity2.assign<Target>(10.0, FACTION_DUMMY);



	int enemies = 0;
	for (int i = 0; i < 0; i++) {
		auto entity = ex.entities.create();
		glm::vec3 pos(rand() % 100, 4500, rand() % 100);
		glm::quat orien(rand() % 100, rand() % 100, rand() % 100, rand() % 100);
		entity.assign<Transform>(pos, normalize(orien));
		entity.assign<Physics>(1000.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
		entity.assign <ModelComponent>(AssetLoader::getLoader().getModel("MIG-212A"));
		entity.assign <FlightComponent>(200.f, 1.5f);
		entity.assign<Target>(10.0, FACTION_AI);
		std::vector<Behaviour*> behaviours;

		std::vector<glm::vec3> plotter;
		plotter.push_back(glm::vec3(2500, 2500, 0));
		plotter.push_back(glm::vec3(2500, 2500, 2500));
		plotter.push_back(glm::vec3(0, 2500, 2500));
		plotter.push_back(glm::vec3(0, 2500, 0));

		//behaviours.push_back(new Constant_Turn(0));
		behaviours.push_back(new Follow_Path(1, new Always_True(), plotter, true));
		behaviours.push_back(new Hunt_Target(2, new Enemy_Close(5000.f), entity_p, 0.05f, 500.f));
		behaviours.push_back(new Hunt_Target(3, new Always_True(), entity2, 0.05, 500.f));
		behaviours.push_back(new Fly_Up(10, new Ground_Close_Front(4.f, 10)));
		behaviours.push_back(new Avoid_Closest(9, new Entity_Close(20.f)));
		behaviours.push_back(new Form_On_Formation(8, new Always_True(), entity_formation));

		entity.assign<AIComponent>(behaviours, true, true);
		entity.assign<CollisionComponent>();
		entity.assign<SoundComponent>(*flyingSB);
		entity.assign<BurstSoundComponent>(*machinegunSB);

		WeaponStats MGstats = WeaponStats(10000, 3, 500, 0.2, 0.02f, true);
		WeaponStats rocketpodstat = WeaponStats(14, 100, 700, 0.2, 0.5f, false);
		std::vector<Weapon> primary;
		std::vector<Weapon> secondary;
		secondary.emplace_back(rocketpodstat, AssetLoader::getLoader().getModel("rocketpod"), AssetLoader::getLoader().getModel("stinger"), glm::vec3(-0.9, -0.37, -1.5), glm::vec3(0.2), glm::vec3(0.8f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);
		secondary.emplace_back(rocketpodstat, AssetLoader::getLoader().getModel("rocketpod"), AssetLoader::getLoader().getModel("stinger"), glm::vec3(0.9, -0.37, -1.5), glm::vec3(0.2), glm::vec3(0.8f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);

		primary.emplace_back(MGstats, AssetLoader::getLoader().getModel("gunpod"), AssetLoader::getLoader().getModel("bullet"), glm::vec3(-0.0, -0.5, 1.0), glm::vec3(0.5), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)));
		entity.assign<Equipment>(primary, secondary);
		entity.assign<PointComponent>(100);
		
		enemies++;
		//std::cout << "Enemy added\n";
	}
	std::cout << "\nAdded " << enemies << " enemies.\n";

	//entity = ex.entities.create();
	//entity.assign<SoundComponent>(soundBuffer);


	// ---	PLAYER	---
	/*entity = ex.entities.create();
	x = 500;
	z = 500;
	//glm::vec3 pos(x, 2500, z);
	//glm::quat orien(1,0,0,0);
	entity.assign<Transform>(pos, normalize(orien));
	entity.assign<Physics>(1000.0, 1.0, glm::vec3(v(), v(), v()), glm::vec3(0.0, 0.0, 0.0));
	entity.assign <ModelComponent>(AssetLoader::getLoader().getModel("MIG-212A"));
	entity.assign <PlayerComponent>();
	entity.assign <FlightComponent>(300.f, 3.f);
	entity.assign <CollisionComponent>();
	entity.assign<SoundComponent>(*flyingSB);
	entity.assign<BurstSoundComponent>(*machinegunShortSB);
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


	entity.assign <Equipment>(pweapons, weapons);*/


	entityx::Entity terrain = ex.entities.create();
	terrain.assign<Terrain>(AssetLoader::getLoader().getHeightmap("testmap"));
}

void PlayingState::update(double dt)
{
	if (deltatime.elapsed() > 30) {
		deltatime.restart();
		spawnEnemies(1);
	}

	glClearColor(100.0/255,149.0/255,234.0/255, 1.0);

	/*
	ex.systems.update<System class here>(dt);
	*/
	
	if (Input::isKeyPressed(GLFW_KEY_ESCAPE)) {
		this->menuOpen = !this->menuOpen;

		if (this->menuOpen) {
			ComponentHandle<SoundComponent> sound;
			for (entityx::Entity entity : ex.entities.entities_with_components(sound)) {
				sound = entity.component<SoundComponent>();
				SoundComponent* s = sound.get();

				s->sound.pause();
			}

			ComponentHandle<BurstSoundComponent> burstSound;
			for (Entity entity : ex.entities.entities_with_components(burstSound)) {
				burstSound = entity.component<BurstSoundComponent>();

				BurstSoundComponent* s = burstSound.get();

				s->sound.stop();
			}
		}
		else {
			ComponentHandle<SoundComponent> sound;
			for (entityx::Entity entity : ex.entities.entities_with_components(sound)) {
				sound = entity.component<SoundComponent>();
				SoundComponent* s = sound.get();

				if (s->sound.getStatus() == s->sound.Paused) {
					s->sound.play();
					//s->sound.setLoop(true);
				}
			}
		}
	}
	
	
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
			AssetLoader::getLoader().getBigtext()->drawText("WASTED", glm::vec2(500, 500), glm::vec3(1, 0, 0), 1.5);
			drawHighscore();
		}
		Window::getWindow().showCursor(true);
		bHandler.drawButtons();
		bHandler.handleButtonClicks();
	}
	
	

	glm::vec2 pos = glm::vec2(10, Window::getWindow().size().y - 20);
	AssetLoader::getLoader().getText()->drawText("Score: " + std::to_string(int(points)), pos, glm::vec3(1, 0, 0), 0.4);

	if (Input::isKeyPressed(GLFW_KEY_F5)) {
		this->changeState(new PlayingState(name));
	}
}

void PlayingState::gameOver() {
	playerAlive = false;
	Highscore::getHighscore().addScore(name, points);
	Highscore::getHighscore().writeToFile();

	ComponentHandle<SoundComponent> sound;
	for (entityx::Entity entity : ex.entities.entities_with_components(sound)) {
		sound = entity.component<SoundComponent>();
		SoundComponent* s = sound.get();

		s->sound.pause();
	}
	/*Highscore list;
	glm::vec2 pos = Window::getWindow().size();
	pos.x = pos.x / 2 - 20;
	pos.y -= 50;
	AssetLoader::getLoader().getText()->drawText("HIGH SCORES", pos, glm::vec3(1, 0, 0), 0.4);*/
	//this->changeState(new GameOverState(name, points));
}

