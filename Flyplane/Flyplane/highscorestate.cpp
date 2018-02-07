#include "highscorestate.h"
#include "backtomenuaction.h"
#include "window.h"

void HighscoreState::init() {
	bHandler.addButton(new Button("Back to menu", glm::vec2(100, 100), glm::vec2(220, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new BackToMenuAction(this), "buttonback"));
}

void HighscoreState::update(double dt) {
	glClearColor(33.0 / 255, 33.0 / 255, 33.0 / 255, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	AssetLoader::getLoader().getBigtext()->drawText("Highscore", glm::vec2(100, Window::getWindow().size().y - 100), glm::vec3(1, 1, 1), 1);
	bHandler.drawButtons();
	bHandler.handleButtonClicks();
}

void HighscoreState::startMenu() {
	this->changeState(new MenuState());
}
