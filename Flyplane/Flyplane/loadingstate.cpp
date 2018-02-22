#include "menustate.h"
#include "backtomenuaction.h"
#include "window.h"
#include "loadingstate.h"
#include "playingstate.h"
#include "startgameaction.h"
#include "input.h"

void LoadingState::init() {
	AssetLoader::getLoader().clearTerrains();
	AssetLoader::getLoader().loadSound("Assets/Sound/button.wav", "buttonsound");
	AssetLoader::getLoader().loadSound("Assets/Sound/buttonforward.wav", "buttonforward");
	AssetLoader::getLoader().loadSound("Assets/Sound/buttonback.wav", "buttonback");
	AssetLoader::getLoader().loadSound("assets/Sound/buttonback.wav", "loadingmusic");


	//bHandler.addButton(new Button("Start", glm::vec2(Window::getWindow().size().x - 200, Window::getWindow().size().y - 100), glm::vec2(210, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new StartGameAction(this), "buttonforward"));
	fi.load("assets/textures/loadingscreen.png");


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
		AssetLoader::getLoader().getBigtext()->drawText("Loading " + current + "...", glm::vec2(100, 100), glm::vec3(1, 1, 1), 1);
	else
		AssetLoader::getLoader().getBigtext()->drawText("Loading done. Press enter to start.", glm::vec2(100, 100), glm::vec3(1, 1, 1), 1);
	if (done) {
		if (Input::isKeyPressed(GLFW_KEY_ENTER)) {
			startGame();
		}
		bHandler.drawButtons();
		bHandler.handleButtonClicks();
	}
}