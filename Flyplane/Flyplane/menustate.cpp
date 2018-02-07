#include "menustate.h"
#include "input.h"
#include "playingstate.h"
#include "window.h"
#include "startgameaction.h"
#include "optionsaction.h"
#include "loadoutaction.h"
#include "assetloader.h"
#include "highscoreaction.h"
#include "highscorestate.h"
#include "loadoutstate.h"
#include "startloadingaction.h"

MenuState::~MenuState() {
}

void MenuState::init() {

	AssetLoader::getLoader().clearTerrains();
	AssetLoader::getLoader().loadSound("Assets/Sound/button.wav", "buttonsound");
	AssetLoader::getLoader().loadSound("Assets/Sound/buttonforward.wav", "buttonforward");
	AssetLoader::getLoader().loadSound("Assets/Sound/buttonback.wav", "buttonback");
	AssetLoader::getLoader().loadSound("assets/Sound/loadingmusic.wav", "loadingmusic");
	bHandler.addButton(new Button("Start", glm::vec2(100, 100), glm::vec2(100,36), glm::vec3(1,1,1), glm::vec3(0.5, 0.5, 0.5), new StartLoadingAction(this), "buttonforward"));
	bHandler.addButton(new Button("Loadout", glm::vec2(100, 150), glm::vec2(150, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new LoadoutAction(this), "buttonforward"));
	bHandler.addButton(new Button("Options", glm::vec2(100, 200), glm::vec2(140, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new OptionsAction(this), "buttonforward"));
	bHandler.addButton(new Button("Highscore", glm::vec2(100, 250), glm::vec2(170, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new HighscoreAction(this), "buttonforward"));
	Window::getWindow().showCursor(true);
}

void MenuState::startOptions() {
	this->changeState(new OptionsState());
}

void MenuState::startHighscore() {
	this->changeState(new HighscoreState());
}

void MenuState::startGame() {
	this->changeState(new LoadingState());
}

void MenuState::startLoadout() {
	this->changeState(new LoadoutState());
}

void MenuState::update(double dt) {
	glClearColor(33.0 / 255, 33.0 / 255, 33.0 / 255, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	bHandler.drawButtons();
	bHandler.handleButtonClicks();
	if (Input::isKeyPressed(GLFW_KEY_ENTER)) {
		this->changeState(new PlayingState());
	}

}
