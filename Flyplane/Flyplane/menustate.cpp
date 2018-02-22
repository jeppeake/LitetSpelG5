#include "menustate.h"
#include "input.h"
#include "window.h"
#include "startgameaction.h"
#include "optionsaction.h"
#include "loadoutaction.h"
#include "assetloader.h"
#include "highscoreaction.h"
#include "highscorestate.h"
#include "loadoutstate.h"
#include "startloadingaction.h"
#include "namestate.h"
#include "physicssystem.h"
#include "transform.h"
#include "rendersystem.h"
#include "weaponsystem.h"
#include "flightsystem.h"
#include "aisystem.h"
#include "particlesystem.h"
#include "physics.h"
#include "flightcomponent.h"
#include "collisioncomponent.h"
#include "modelcomponent.h"

#include "behaviour.h"
#include "follow_path.h"
#include "follow_player.h"
#include "always_true.h"
#include "hunt_target.h"
#include "fly_up.h"
#include "avoid_closest.h"
#include "form_on_formation.h"
#include "entity_close.h"
#include "ground_close_front.h"

#include "mission.h"

entityx::Entity entity3;

void MenuState::spawnEnemies(int nr) {

	for (int i = 0; i < nr; i++) {
		auto entity = ex.entities.create();
		float x = rand() % 2000 - 1000;
		float z = rand() % 2000;
		glm::vec3 pos(x, AssetLoader::getLoader().getHeightmap("testmap")->heightAt(glm::vec3(x, 0, z)) + 1500, z);
		glm::quat orien(rand() % 100, rand() % 100, rand() % 100, rand() % 100);
		entity.assign<Transform>(pos, normalize(orien));
		entity.assign<Physics>(1000.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
		entity.assign <ModelComponent>(AssetLoader::getLoader().getModel("MIG-212A"));
		entity.assign <FlightComponent>(100.f, 200.f, 50.f, 1.5f, 0.5f);
		entity.assign<Target>(10.0, FACTION_AI);
		entity.assign <HealthComponent>(100.0);
		auto handle = entity.assign<ParticleComponent>();
		ex.events.emit<AddParticleEvent>(PARTICLE_TYPES::TRAIL, handle);
		ex.events.emit<AddParticleEvent>(PARTICLE_TYPES::ENGINE_TRAIL, handle);

		std::vector<Behaviour*> behaviours;

		std::vector<glm::vec3> plotter;
		plotter.push_back(glm::vec3(2500, 4500, 0));
		plotter.push_back(glm::vec3(2500, 4500, 2500));
		plotter.push_back(glm::vec3(0, 4500, 2500));
		plotter.push_back(glm::vec3(0, 4500, 0));

		//behaviours.push_back(new Constant_Turn(0));
		//behaviours.push_back(new Follow_Path(1, new Always_True(), plotter, true));
		behaviours.push_back(new Hunt_Target(3, new Always_True(), entity3, 0.05, 500.f));
		behaviours.push_back(new Fly_Up(10, new Ground_Close_Front(4.f, 10)));
		behaviours.push_back(new Avoid_Closest(9, new Entity_Close(40.f)));

		entity.assign<AIComponent>(behaviours, true, true, false);
		entity.assign<CollisionComponent>();

		WeaponStats MGstats = WeaponStats(10000, 3, 500, 0.2, 0.02f, true);
		std::vector<Weapon> primary;
		std::vector<Weapon> secondary;

		primary.emplace_back(MGstats, AssetLoader::getLoader().getModel("gunpod"), AssetLoader::getLoader().getModel("bullet"), glm::vec3(-0.0, -0.5, 1.0), glm::vec3(0.5), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)));
		entity.assign<Equipment>(primary, secondary);
	}
}

MenuState::~MenuState() {
}

void MenuState::init() {

	vignette.load("assets/Textures/vignette.png");
	AssetLoader::getLoader().loadSound("Assets/Sound/hover.wav", "buttonsound");
	AssetLoader::getLoader().loadSound("Assets/Sound/hammerclick.wav", "buttonforward");
	AssetLoader::getLoader().loadSound("Assets/Sound/hammerclick.wav", "buttonback");
	AssetLoader::getLoader().loadSound("assets/Sound/buttonback.wav", "loadingmusic");
	bHandler.addButton(new Button("START", glm::vec2(100, 100), glm::vec2(100,36), glm::vec3(1,1,1), glm::vec3(0.5, 0.5, 0.5), new StartLoadingAction(this), "buttonforward"));
	bHandler.addButton(new Button("LOADOUT", glm::vec2(100, 150), glm::vec2(150, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new LoadoutAction(this), "buttonforward"));
	bHandler.addButton(new Button("OPTIONS", glm::vec2(100, 200), glm::vec2(140, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new OptionsAction(this), "buttonforward"));
	bHandler.addButton(new Button("HIGHSCORE", glm::vec2(100, 250), glm::vec2(170, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new HighscoreAction(this), "buttonforward"));
	Window::getWindow().showCursor(true);

	//load all assets, all assets are given a reference name to used when retreiving it
	AssetLoader::getLoader().loadModel("assets/bullet.fbx", "bullet");
	AssetLoader::getLoader().loadModel("assets/Weapons/Guns/37mm_gunpod/37mm_gunpod.fbx", "gunpod");
	AssetLoader::getLoader().loadModel("assets/Weapons/Missiles/Fishrod/fishrod.fbx", "fishrod");
	AssetLoader::getLoader().loadModel("assets/MIG-212A.fbx", "MIG-212A");
	AssetLoader::getLoader().loadModel("Weapons/Missiles/ALAAT-10/ALAAT-10.fbx", "ALAAT-10");
	AssetLoader::getLoader().loadModel("assets/buildings/911.fbx", "hus1");

	AssetLoader::getLoader().loadHeightmap("assets/Terrain/map.txt", "testmap");

	//AssetLoader::getLoader().loadSound("assets/Sound/airplane-takeoff.wav", "takeoff");
	/*AssetLoader::getLoader().loadSound("assets/Sound/airplane1.wav", "takeoff");
	AssetLoader::getLoader().loadSound("assets/Sound/Missle_Launch.wav", "missile");
	AssetLoader::getLoader().loadSound("assets/Sound/Sniper_Rifle_short.wav", "sniperrifle");
	AssetLoader::getLoader().loadSound("assets/Sound/Machine_gun.wav", "machinegun");
	AssetLoader::getLoader().loadSound("assets/Sound/Machine_gun_short.wav", "machinegunShort");


	//get all assets (not really needed, can be used inline)
	flyingSB = AssetLoader::getLoader().getSoundBuffer("takeoff");
	missileSB = AssetLoader::getLoader().getSoundBuffer("missile");
	bulletSB = AssetLoader::getLoader().getSoundBuffer("sniperrifle");
	machinegunSB = AssetLoader::getLoader().getSoundBuffer("machinegun");
	machinegunShortSB = AssetLoader::getLoader().getSoundBuffer("machinegunShort");*/

	/*
	* add systems
	*
	* example

	ex.systems.add<System class here>();
	*/

	Camera camera;
	Transform trans;
	trans.pos = glm::vec3(0, AssetLoader::getLoader().getHeightmap("testmap")->heightAt(glm::vec3(0, 0, 0)) + 1500, 0);
	camera.setTransform(trans);
	Renderer::getRenderer().setCamera(camera);

	ex.systems.add<PhysicsSystem>();
	ex.systems.add<WeaponSystem>();
	ex.systems.add<RenderSystem>();
	ex.systems.system<RenderSystem>()->cullingCamera = camera;
	ex.systems.system<RenderSystem>()->playing = false;
	//ex.systems.add<PlayerSystem>();
	ex.systems.add<FlightSystem>();
	ex.systems.add<AISystem>();
	ex.systems.add<ParticleSystem>();

	ex.systems.configure();

	entity3 = ex.entities.create();
	float x = 0;
	float z = 100;
	glm::vec3 pos(x, 2500, z);
	glm::quat orien(1, 0, 0, 0);
	entity3.assign<Transform>(pos, normalize(orien));
	entity3.assign<Physics>(1000.0, 1.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
	entity3.assign <ModelComponent>(AssetLoader::getLoader().getModel("MIG-212A"));
	entity3.assign <FlightComponent>(100.f, 200.f, 50.f, 1.5f, 0.5f);
	entity3.assign <CollisionComponent>();
	entity3.assign<PlayerComponent>();
	auto handle = entity3.assign<ParticleComponent>();
	ex.events.emit<AddParticleEvent>(PARTICLE_TYPES::TRAIL, handle);
	ex.events.emit<AddParticleEvent>(PARTICLE_TYPES::ENGINE_TRAIL, handle);

	std::vector<Behaviour*> behaviours;

	std::vector<glm::vec3> plotter;
	plotter.push_back(glm::vec3(1000, 2500, 1000));
	plotter.push_back(glm::vec3(-1000, 2500, 1000));
	plotter.push_back(glm::vec3(-1000, 2500, 500));
	plotter.push_back(glm::vec3(1000, 2500, 500));

	
	//behaviours.push_back(new Constant_Turn(0));
	behaviours.push_back(new Follow_Path(1, new Always_True(), plotter, true));

	entity3.assign<AIComponent>(behaviours, true, true, false);
	entity3.assign<Target>(10.0, FACTION_DUMMY);
	entity3.assign <HealthComponent>(100.0);

	spawnEnemies(5);

	entityx::Entity terrain = ex.entities.create();
	terrain.assign<Terrain>(AssetLoader::getLoader().getHeightmap("testmap"));
	AssetLoader::getLoader().getHeightmap("testmap")->buildStructures(ex.entities);
}

void MenuState::startOptions() {
	this->changeState(new OptionsState());
}

void MenuState::startHighscore() {
	this->changeState(new HighscoreState());
}

void MenuState::startGame() {
	this->changeState(new NameState());
}

void MenuState::startLoadout() {
	this->changeState(new LoadoutState());
}

void MenuState::update(double dt) {
	glClearColor(100.0 / 255, 149.0 / 255, 234.0 / 255, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	ex.systems.update<AISystem>(dt);
	ex.systems.update<FlightSystem>(dt);
	ex.systems.update<PhysicsSystem>(dt);
	ex.systems.update<WeaponSystem>(dt);
	//ex.systems.update<SoundSystem>(dt);
	ex.systems.update<ParticleSystem>(dt);
	ex.systems.update<RenderSystem>(dt);
	glClear(GL_DEPTH_BUFFER_BIT);
	vignette.bind();
	vignette.draw();
	glClear(GL_DEPTH_BUFFER_BIT);
	bHandler.drawButtons();
	bHandler.handleButtonClicks();
}
