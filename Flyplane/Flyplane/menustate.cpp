#include "menustate.h"
#include "input.h"
#include "playingstate.h"
#include "window.h"
#include "startgameaction.h"
#include "optionsaction.h"
#include "assetloader.h"
#include "highscoreaction.h"
#include "highscorestate.h"

MenuState::~MenuState() {
}

void MenuState::init() {
	bHandler.addButton(new Button("Start", glm::vec2(100, 100), glm::vec2(100,36), glm::vec3(1,1,1), glm::vec3(0.5, 0.5, 0.5), new StartGameAction(this)));
	bHandler.addButton(new Button("Loadout", glm::vec2(100, 150), glm::vec2(150, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new StartGameAction(this)));
	bHandler.addButton(new Button("Options", glm::vec2(100, 200), glm::vec2(110, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new OptionsAction(this)));
	bHandler.addButton(new Button("Highscore", glm::vec2(100, 250), glm::vec2(110, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new HighscoreAction(this)));
	Window::getWindow().showCursor(true);
}

void MenuState::startOptions() {
	this->changeState(new OptionsState());
}

void MenuState::startHighscore() {
	this->changeState(new HighscoreState());
}

void MenuState::startGame() {
	this->changeState(new PlayingState());
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
