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
#include "equipment.h"
#include "collisionsystem.h"
#include "soundsystem.h"
#include "gameoversystem.h"
#include "particlesystem.h"
#include "camerasystem.h"

#include "dropcomponent.h"
#include "aicomponent.h"
#include "aisystem.h"
#include "behaviour.h"
#include "constant_turn.h"
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

#include "turret.h"

#include "menustate.h"
#include "pointcomponent.h"
#include <string>
#include "window.h"
#include "highscore.h"
#include "backtomenuaction.h"
#include "restartaction.h"
#include "pauseEvent.h"

#include "weaponpreset.h"
#include "planepreset.h"
#include <fstream>

#include "healthcomponent.h"
#include "healthsystem.h"

#include "mission.h"
#include "missionsystem.h"

#include "lifetimesystem.h"
#include "musicmanager.h"

//entityx::Entity entity;

entityx::Entity entity_formation;
entityx::Entity entity2;

//sf::SoundBuffer* missileSB;

std::vector<Weapon> Equipment::playerLoadout;

void PlayingState::spawnEnemies(int nr) {



	for (int i = 0; i < nr; i++) {
		auto entity = ex.entities.create();
		float x = rand() % 1000;
		float z = rand() % 1000;
		glm::vec3 pos(x, AssetLoader::getLoader().getHeightmap("testmap")->heightAt(glm::vec3(x, 0, z)) + 1500, z);
		glm::quat orien(rand() % 100, rand() % 100, rand() % 100, rand() % 100);
		entity.assign<Transform>(pos, normalize(orien));
		entity.assign<Physics>(1000.0, 0.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
		entity.assign <ModelComponent>(AssetLoader::getLoader().getModel("MIG-212A"));
		entity.assign <FlightComponent>(100.f, 200.f, 50.f, 1.f, 0.5f);
		entity.assign<Target>(10.0, FACTION_AI);
		entity.assign <HealthComponent>(100.0);
		entity.assign<FactionEnemy>();

		auto handle = entity.assign<ParticleComponent>();
		ex.events.emit<AddParticleEvent>(TRAIL, handle);
		ex.events.emit<AddParticleEvent>(ENGINE_TRAIL, handle);
		std::vector<Behaviour*> behaviours;

		std::vector<glm::vec3> plotter;
		plotter.push_back(glm::vec3(2500, 4500, 0));
		plotter.push_back(glm::vec3(2500, 4500, 2500));
		plotter.push_back(glm::vec3(0, 4500, 2500));
		plotter.push_back(glm::vec3(0, 4500, 0));

		//behaviours.push_back(new Constant_Turn(0));
		behaviours.push_back(new Follow_Path(1, new Always_True(), plotter, true));
		behaviours.push_back(new Hunt_Target(2, new Enemy_Close(5000.f), entity_p, 0.05f, 500.f));
		behaviours.push_back(new Hunt_Target(3, new Always_True(), entity2, 0.05, 500.f));
		behaviours.push_back(new Fly_Up(10, new Ground_Close_Front(4.f, 10)));
		behaviours.push_back(new Avoid_Closest(9, new Entity_Close(40.f)));
		behaviours.push_back(new Form_On_Formation(8, new Always_True(), entity_formation));

		entity.assign<AIComponent>(behaviours, true, true, false, true);
		entity.assign<CollisionComponent>();
		entity.assign<SoundComponent>(*flyingSB);
		entity.assign<BurstSoundComponent>(*machinegunSB);

		WeaponStats MGstats = WeaponStats(10000, 3, 350, 0.2, 0.02f, true);
		WeaponStats rocketpodstat = WeaponStats(14, 100, 700, 0.2, 0.5f, false);
		std::vector<Weapon> primary;
		std::vector<Weapon> secondary;
		//secondary.emplace_back(rocketpodstat, AssetLoader::getLoader().getModel("rocketpod"), AssetLoader::getLoader().getModel("stinger"), glm::vec3(-0.9, -0.37, -1.5), glm::vec3(0.2), glm::vec3(0.8f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);
		//secondary.emplace_back(rocketpodstat, AssetLoader::getLoader().getModel("rocketpod"), AssetLoader::getLoader().getModel("stinger"), glm::vec3(0.9, -0.37, -1.5), glm::vec3(0.2), glm::vec3(0.8f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);

		primary.emplace_back(MGstats, AssetLoader::getLoader().getModel("gunpod"), AssetLoader::getLoader().getModel("bullet"), glm::vec3(-0.0, -0.5, 1.0), glm::vec3(0.5), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)));
		entity.assign<Equipment>(primary, secondary);
		entity.assign<PointComponent>(100);
	}
}

void PlayingState::spawnDrop(DropComponent::TypeOfDrop typeOfDrop) {
	std::cout << "Drop spawned!";

	std::string model;
	glm::vec3 pos(rand() % 10000 - 5000, 0, rand() % 10000 - 5000);

	if (typeOfDrop == DropComponent::Ammo)
		model = "Ammo_sign";
	else if (typeOfDrop == DropComponent::Health)
		model = "Repair_sign";

	ComponentHandle<PlayerComponent> player;
	ComponentHandle<Transform> transform;
	for (entityx::Entity entity : ex.entities.entities_with_components(transform, player)) {
		float distance = glm::distance(pos, glm::vec3(transform->pos.x, 0, transform->pos.z));

		if (distance > 2500) {
			glm::vec3 direction = glm::normalize(pos - glm::vec3(transform->pos.x, 0, transform->pos.z));
			pos = glm::vec3(transform->pos.x, transform->pos.y, transform->pos.z) + direction * 2500.f;
		}
	}

	auto entity = ex.entities.create();
	pos.y = AssetLoader::getLoader().getHeightmap("testmap")->heightAt(pos) + 2500;
	auto handle = entity.assign<Transform>(pos, glm::quat(1, 0, 0, 0));
	handle->scale = glm::vec3(75.0, 75.0, 75.0);
	entity.assign<ModelComponent>(AssetLoader::getLoader().getModel(model));
	entity.assign<CollisionComponent>();
	entity.assign<DropComponent>(typeOfDrop);
	entity.assign<LifeTimeComponent>(60.0);
}

void PlayingState::drawHighscore() {
	glm::vec2 pos;
	pos.x = Window::getWindow().size().x/2 - 80;
	pos.y = Window::getWindow().size().y - 300;
	string* p = Highscore::getHighscore().getHighscoreList();
	AssetLoader::getLoader().getHighscoreText()->drawText("HIGH SCORES", pos, glm::vec3(1, 1, 1), 0.8);
	pos.x = Window::getWindow().size().x / 2 - 170;
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
	pp.preorder = true;
	std::getline(file, str);
	pp.load(str);

	float x = 0;
	float z = 0;
	float y = AssetLoader::getLoader().getHeightmap("testmap")->heightAt(glm::vec3(x, 0, z)) + 1500;
	glm::vec3 pos(x, y, z);
	glm::quat orien(1, 0, 0, 0);
	//std::cout << orien.x << orien.y << orien.z << orien.w << "\n";
	// ---	PLAYER	---
	entity_p = ex.entities.create();
	x = 500;
	z = 500;
	//glm::vec3 pos(x, 2500, z);
	//glm::quat orien(1,0,0,0);

	AssetLoader::getLoader().loadModel(pp.model, pp.name);

	entity_p.assign<Transform>(pos, normalize(orien));
	entity_p.assign<Physics>(1000.0, 0.0, glm::vec3(v(), v(), v()), glm::vec3(0.0, 0.0, 0.0));
	entity_p.assign <ModelComponent>(AssetLoader::getLoader().getModel(pp.name));
	entity_p.assign <PlayerComponent>();
	entity_p.assign <CameraOnComponent>();
	entity_p.assign <FlightComponent>(pp.normalspeed, pp.boostspeed, pp.breakforce, pp.turnrate, pp.acceleration);
	entity_p.assign <CollisionComponent>();
	entity_p.assign<SoundComponent>(*flyingSB);
	entity_p.assign<BurstSoundComponent>(*machinegunShortSB);
	entity_p.assign<Target>(10.0, FACTION_PLAYER);
	entity_p.assign<FactionPlayer>();

	auto handle = entity_p.assign<ParticleComponent>();
	ex.events.emit<AddParticleEvent>(TRAIL, handle);
	ex.events.emit<AddParticleEvent>(SPEED_PARTICLE, handle);

	

	ParticleParameters p;
	p.engineTrail.radius = pp.engineRadius;
	for (auto pos : pp.enginePos) {
		p.engineTrail.offset = pos;
		ex.events.emit<AddParticleEvent>(ENGINE_TRAIL, handle, p);
	}
	for (auto pos : pp.wingTrailPos) {
		p.wingTrail.respawnCounter = 0;
		p.wingTrail.offset = pos;
		ex.events.emit<AddParticleEvent>(WING_TRAIL, handle, p);
	}


	std::vector<Weapon> weapons;
	std::vector<Weapon> pweapons;

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
	std::vector<Turret> turrets;
	if (pp.weapon != "nan") {
		if (pp.turretWeapon) {//turret
			TurretPreset TP;
			TP.load(pp.weapon);
			//std::cout << "Loaded: " << this->planePresets[this->selected].weapon << "\n";
			turrets.emplace_back(TP.getTurret());
		}
		else {//primary
			WeaponPreset PW;
			PW.load(pp.weapon);
			//std::cout << "Loaded: " << this->planePresets[this->selected].weapon << "\n";
			WeaponStats stats = WeaponStats(PW.ammo, PW.lifetime, PW.speed, PW.mass, PW.cooldown, PW.infAmmo);
			pweapons.emplace_back(stats, AssetLoader::getLoader().getModel(PW.name), AssetLoader::getLoader().getModel(PW.projModel), PW.extraOffset, glm::vec3(PW.scale), glm::vec3(PW.projScale), glm::angleAxis(0.f, glm::vec3(0, 0, 1)), false, false);
		}
	}


	//turret load
	for (int i = 0; i < pp.turretFiles.size(); i++) {
		TurretPreset TP;
		TP.load(pp.turretFiles[i]);
		Turret turret = TP.getTurret();
		turret.placement.offset = pp.turretPositions[i];
		turret.placement.orientation = glm::quat(glm::radians(pp.turretOrientations[i]));
		turret.placement.front = pp.turretFronts[i];
		turrets.emplace_back(turret);
	}

	std::getline(file, str);
	entity_p.component<ModelComponent>().get()->mptr->texture = *AssetLoader::getLoader().getTexture(pp.textureNames[std::stoi(str)]);

	WeaponStats stats2 = WeaponStats(10000, 3, 350.f, 0.2, 0.005f, true, 8);
	//pweapons.emplace_back(stats2, AssetLoader::getLoader().getModel("gunpod"), AssetLoader::getLoader().getModel("bullet"), glm::vec3(-0.0, -0.25, 0.5), glm::vec3(0.25), glm::vec3(3.f, 3.f, 6.f), glm::angleAxis(0.f, glm::vec3(0, 0, 1)));
	WeaponStats bomb = WeaponStats(10, 1000000000, 0, 100, 0.5f, true);
	//weapons.emplace_back(bomb, AssetLoader::getLoader().getModel("bullet"), AssetLoader::getLoader().getModel("fishrod"), glm::vec3(0, -0.3, -0.1));

	//TurretInfo info(180.f, glm::vec2(35.f, 35.f), glm::vec2(90.f, 90.f), 1000.f, AssetLoader::getLoader().getModel("spectre_mount"), AssetLoader::getLoader().getModel("spectre_gun"));
	//TurretPlacement placement(glm::normalize(orien), glm::vec3(1.f), glm::vec3(0.f, -0.32f, 1.5f), glm::vec3(0.f, 0.f, 1.f));
	//WeaponInfo WInfo(glm::vec3(3.f), AssetLoader::getLoader().getModel("bullet"));
	//turrets.emplace_back(stats2, info, placement, WInfo, false);

	Equipment::setPlayerLoadout(weapons);
	entity_p.assign <Equipment>(pweapons, weapons, turrets);
	entity_p.assign <HealthComponent>(1000.0);

	entityx::Entity terrain = ex.entities.create();

}

void PlayingState::startMenu() {
	this->changeState(new MenuState());
	MusicManager::getMusicManager().playMenuMusic();
}

void PlayingState::restart() {
	this->changeState(new PlayingState(name));
}




void PlayingState::init() 
{

	srand(static_cast<unsigned>(time(NULL)));
	Window::getWindow().showCursor(true);

	bHandler.addButton(new Button("Restart", glm::vec2(100, 100), glm::vec2(120, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new RestartAction(this), "buttonforward"));
	bHandler.addButton(new Button("Back to menu", glm::vec2(100, 150), glm::vec2(200, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new BackToMenuAction(this), "buttonback"));

	MusicManager::getMusicManager().playPlayingMusic();

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
	AssetLoader::getLoader().loadModel("assets/buildings/bighus.fbx", "hus1");
	AssetLoader::getLoader().loadModel("assets/buildings/kub.fbx", "kub");
	AssetLoader::getLoader().loadModel("assets/misc/Ammo_sign.fbx", "Ammo_sign");
	AssetLoader::getLoader().loadModel("assets/misc/Repair_sign.fbx", "Repair_sign");

	//AssetLoader::getLoader().loadHeightmap("assets/Terrain/map.txt", "testmap");

	//AssetLoader::getLoader().loadSound("assets/Sound/airplane-takeoff.wav", "takeoff");
	AssetLoader::getLoader().loadSound("assets/Sound/airplane1.wav", "takeoff");
	AssetLoader::getLoader().loadSound("assets/Sound/Missle_Launch.wav", "missile");
	AssetLoader::getLoader().loadSound("assets/Sound/Sniper_Rifle_short.wav", "sniperrifle");
	AssetLoader::getLoader().loadSound("assets/Sound/Machine_gun.wav", "machinegun");
	//AssetLoader::getLoader().loadSound("assets/Sound/Machine_gun_short.wav", "machinegunShort");
	AssetLoader::getLoader().loadSound("assets/Sound/Machine_gun_shorter.wav", "machinegunShort");


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
	ex.systems.add<HealthSystem>(this);
	ex.systems.add<GameOver>(this);
	ex.systems.add<ParticleSystem>();
	ex.systems.add<CameraSystem>();
	ex.systems.add<LifeTimeSystem>();
	ex.systems.add<MissionSystem>(this);
	ex.systems.configure();

	/*
	* add entities
	*
	* example

	auto entity = ex.entities.create();
	entity.assign<Component class here>(Component constructor params);
	*/
	
	loadLoadout();

	entityx::Entity terrain = ex.entities.create();
	terrain.assign<Terrain>(AssetLoader::getLoader().getHeightmap("testmap"));
	AssetLoader::getLoader().getHeightmap("testmap")->buildStructures(ex.entities);

	/*
	for (int i = 0; i < 200; i++) {
		entityx::Entity house = ex.entities.create();
		glm::vec3 pos = AssetLoader::getLoader().getHeightmap("testmap")->generateHousePos();
		house.assign<Transform>(pos);
		house.assign<ModelComponent>(AssetLoader::getLoader().getModel("hus1"));
	}
	*/

	entityx::Entity tower = ex.entities.create();
	tower.assign<HouseComponent>();
	tower.assign<CollisionComponent>();
	tower.assign<ModelComponent>(AssetLoader::getLoader().getModel("monolith"));
	glm::vec3 pos(3000,0,-7000);
	pos.y = AssetLoader::getLoader().getHeightmap("testmap")->heightAt(pos);
	float size = 200.f;
	tower.assign<Transform>(pos, glm::quat(glm::vec3(0, glm::pi<float>()/4.f, 0)), glm::vec3(size));


}

void PlayingState::update(double dt)
{
	glClearColor(100.0/255,149.0/255,234.0/255, 1.0);
	pointObject.time -= (float)dt;
	/*
	ex.systems.update<System class here>(dt);
	*/

	if (Input::isKeyPressed(GLFW_KEY_ESCAPE)) {
		this->menuOpen = !this->menuOpen;

		if (this->menuOpen) {
			ex.events.emit<PauseEvent>(true, ex);
		}
		else {
			ex.events.emit<PauseEvent>(false, ex);
		}
	}
	
	
	if(Input::isKeyPressed(GLFW_KEY_F8))
		std::cout << ex.entities.size() << "\n";


	

	/*bool playerAlive = false;

	ComponentHandle<PlayerComponent> p_player;
	for (entityx::Entity entity : ex.entities.entities_with_components(p_player)) {
		playerAlive = true;
	}*/

	glm::vec2 window = Window::getWindow().size();



	if (playerAlive && !menuOpen) {
		//points += 10 * dt;
		gt += dt;

		if (gt > 30.0) {
			gt -= 30.0;
			spawnEnemies(spawnCounter);
			spawnCounter++;
			spawnDrop(DropComponent::Ammo);
			//spawnDrop(DropComponent::Ammo);
			//spawnDrop(DropComponent::Ammo);
			spawnDrop(DropComponent::Health);
		}

		timerMultiplier -= dt;
		if (timerMultiplier <= 0 && multiplier > 1) {
			multiplier--;
			timerMultiplier = 5.0;
		}

		Window::getWindow().showCursor(false);
		ex.systems.update<PlayerSystem>(dt);
		ex.systems.update<AISystem>(dt);

		ex.systems.update<FlightSystem>(dt);
		ex.systems.update<PhysicsSystem>(dt);
		ex.systems.update<WeaponSystem>(dt);
		ex.systems.update<SoundSystem>(dt);
		ex.systems.update<CollisionSystem>(dt);
		ex.systems.update<HealthSystem>(dt);
		ex.systems.update<LifeTimeSystem>(dt);

		ex.systems.update<ParticleSystem>(dt);

		ex.systems.update<CameraSystem>(dt);
		ex.systems.update<RenderSystem>(dt);
		ex.systems.update<MissionSystem>(dt);

		if (pointObject.time <= 0) {
			points += pointObject.points;
			pointObject.points = 0;
		}
		
		//AssetLoader::getLoader().getText()->drawText("+" + std::to_string(pointObject.points), glm::vec2(window.x * 0.2f, window.y * 0.95f), glm::vec3(1, 1, 0), 1.0);
	}
	else {
		//ex.systems.update<CameraSystem>(dt);
		ex.systems.update<RenderSystem>(dt);
		Renderer::getRenderer().RenderTransparent();
		if (!playerAlive) {
			Renderer::getRenderer().setIsOutside(false);
			AssetLoader::getLoader().getBigtext()->drawText("Game over", glm::vec2((Window::getWindow().size().x/2) - 9*16, Window::getWindow().size().y - 200), glm::vec3(1, 0, 0), 1.5);
			drawHighscore();
		}
		Window::getWindow().showCursor(true);
		bHandler.drawButtons();
		bHandler.handleButtonClicks();
	}
	
	

	glm::vec2 pos = glm::vec2(130, window.y - 30);
	//AssetLoader::getLoader().getText()->drawText(std::to_string(int(points)), pos, glm::vec3(0, 1, 0), 0.4);
	Renderer::getRenderer().setScore(points);
	//AssetLoader::getLoader().getText()->drawText("X" + std::to_string(multiplier), glm::vec2(10, window.y - 80), glm::vec3(0, 1, 0), 0.4);
	Renderer::getRenderer().setMultiplier(multiplier);
	Renderer::getRenderer().setPoints(pointObject.points);

	if (Input::isKeyPressed(GLFW_KEY_F5)) {
		this->changeState(new PlayingState(name));
	}
}

void PlayingState::gameOver() {
	playerAlive = false;
	Highscore::getHighscore().addScore(name, points);
	Highscore::getHighscore().writeToFile();

	ex.events.emit<PauseEvent>(true, ex);

	/*Highscore list;
	glm::vec2 pos = Window::getWindow().size();
	pos.x = pos.x / 2 - 20;
	pos.y -= 50;
	AssetLoader::getLoader().getText()->drawText("HIGH SCORES", pos, glm::vec3(1, 0, 0), 0.4);*/
	//this->changeState(new GameOverState(name, points));
}

void PlayingState::addPoints(int p) {
	if (pointObject.time > 0) {
		pointObject.points += p * multiplier;
	}

	else {
		pointObject.points = p * multiplier;
	}
	pointObject.time = 0.8;
	//points += pointObject.points;
	if (multiplier <= 7)
		multiplier++;
	timerMultiplier = 5;
}


