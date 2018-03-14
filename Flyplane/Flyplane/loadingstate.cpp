#include "menustate.h"
#include "backtomenuaction.h"
#include "window.h"
#include "loadingstate.h"
#include "playingstate.h"
#include "startgameaction.h"
#include "input.h"
#include "musicmanager.h"

void LoadingState::init() {
	MusicManager::getMusicManager().pause();
	AssetLoader::getLoader().clearTerrains();
	AssetLoader::getLoader().loadSound("Assets/Sound/button.wav", "buttonsound");
	AssetLoader::getLoader().loadSound("Assets/Sound/buttonforward.wav", "buttonforward");
	AssetLoader::getLoader().loadSound("Assets/Sound/buttonback.wav", "buttonback");
	AssetLoader::getLoader().loadSound("assets/Sound/buttonback.wav", "loadingmusic");


	//bHandler.addButton(new Button("Start", glm::vec2(Window::getWindow().size().x - 200, Window::getWindow().size().y - 100), glm::vec2(210, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new StartGameAction(this), "buttonforward"));
	fi.load("assets/textures/loadingbg.png");


	music.setBuffer(*AssetLoader::getLoader().getSoundBuffer("loadingmusic"));
	music.play();
	
	current = "models";
	update(0);
	Window::getWindow().update();

	AssetLoader::getLoader().loadModel("assets/bullet.fbx", "bullet");
	AssetLoader::getLoader().loadModel("assets/basicgun.fbx", "basicgun");
	AssetLoader::getLoader().loadModel("assets/GAU-15.fbx", "GAU-15");
	AssetLoader::getLoader().loadModel("assets/Weapons/Guns/37mm_gunpod/37mm_gunpod.fbx", "gunpod");
	AssetLoader::getLoader().loadModel("assets/Weapons/Missiles/Fishrod/fishrod.fbx", "fishrod");
	AssetLoader::getLoader().loadModel("assets/Weapons/Rocketpod/rocketpod.fbx", "rocketpod");
	AssetLoader::getLoader().loadModel("assets/Weapons/Missiles/Stinger/stinger.fbx", "stinger");
	AssetLoader::getLoader().loadModel("assets/MIG-212A.fbx", "MIG-212A");
	AssetLoader::getLoader().loadModel("assets/Weapons/missiles/ALAAT-10/ALAAT-10.fbx", "ALAAT-10");

	AssetLoader::getLoader().loadModel("assets/buildings/Bunker_small/Bunker_small2.fbx", "bunker");
	AssetLoader::getLoader().loadModel("assets/buildings/Monolith/Monolith.fbx", "monolith");

	AssetLoader::getLoader().loadModel("assets/buildings/911bb.fbx", "testhus");
	AssetLoader::getLoader().loadModel("assets/buildings/hus1.fbx", "hus1");

	AssetLoader::getLoader().loadModel("assets/TU-101/TU-101.fbx", "TU-101");

	AssetLoader::getLoader().loadModel("assets/Spectre/Spectre_GUNS.fbx", "spectre_gun");
	AssetLoader::getLoader().loadModel("assets/Spectre/Spectre_MOUNT.fbx", "spectre_mount");

	AssetLoader::getLoader().loadWeapon("assets/Presets/Weapons/HAAM_166.txt", "HAAM-166");
	AssetLoader::getLoader().loadWeapon("assets/Presets/Weapons/micro.txt", "Micromissile");
	AssetLoader::getLoader().loadWeapon("assets/Presets/Weapons/rocketpod.txt", "Rocketpod");
	AssetLoader::getLoader().loadWeapon("assets/Presets/Weapons/missile.txt", "Fishrod");
	AssetLoader::getLoader().loadWeapon("assets/Presets/Weapons/missile2.txt", "Rodfish");
	AssetLoader::getLoader().loadWeapon("assets/Presets/Weapons/missile3.txt", "Bigboi");
	AssetLoader::getLoader().loadWeapon("assets/Presets/Weapons/SHAAM-200.txt", "SHAAM-200");
	


	current = "terrain";
	update(0);
	Window::getWindow().update();

	AssetLoader::getLoader().loadHeightmap("assets/Terrain/map.txt", "testmap");

	current = "sounds";
	update(0);
	Window::getWindow().update();

	//AssetLoader::getLoader().loadSound("assets/Sound/airplane1.wav", "takeoff");
	AssetLoader::getLoader().loadSound("assets/Sound/avion.wav", "takeoff");
	AssetLoader::getLoader().loadSound("assets/Sound/Missle_Launch.wav", "missile");
	AssetLoader::getLoader().loadSound("assets/Sound/Sniper_Rifle_short.wav", "sniperrifle");
	AssetLoader::getLoader().loadSound("assets/Sound/Machine_gun.wav", "machinegun");
	AssetLoader::getLoader().loadSound("assets/Sound/explosion.wav", "explosion");
	AssetLoader::getLoader().loadSound("assets/Sound/newTink.wav", "tink");
	AssetLoader::getLoader().loadSound("assets/Sound/wind1.wav", "wind");
	AssetLoader::getLoader().loadSound("assets/Sound/ammodrop.wav", "ammodrop");
	AssetLoader::getLoader().loadSound("assets/Sound/healthdrop.wav", "healthdrop");
	AssetLoader::getLoader().loadSound("assets/Sound/playerhit.wav", "playerhit");
	AssetLoader::getLoader().loadSound("assets/Sound/bomb-siren.wav", "warning");
	AssetLoader::getLoader().loadSound("assets/Sound/high-def-machine-gun.wav", "HD-MG");
	AssetLoader::getLoader().loadSound("assets/Sound/mp5-smg.wav", "mp5-smg");
	AssetLoader::getLoader().loadSound("assets/Sound/shooting-an-mp5.wav", "mp5");
	AssetLoader::getLoader().loadSound("assets/Sound/flare.wav", "flare");

	done = true;
}

void LoadingState::startGame() {
	this->changeState(new PlayingState(name));
}

void LoadingState::update(double dt) {
	glClearColor(33.0 / 255, 33.0 / 255, 33.0 / 255, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	fi.bind();
	fi.draw();
	glClear(GL_DEPTH_BUFFER_BIT);
	if (!done)
		AssetLoader::getLoader().getBigtext()->drawText("Loading " + current + "...", glm::vec2(100, 70), glm::vec3(1, 1, 1), 1);
	else
		AssetLoader::getLoader().getBigtext()->drawText("Loading done. Press enter to start.", glm::vec2(100, 70), glm::vec3(1, 1, 1), 1);
	if (done) {
		if (Input::isKeyPressed(GLFW_KEY_ENTER)) {
			startGame();
		}
		bHandler.drawButtons();
		bHandler.handleButtonClicks();
	}
}