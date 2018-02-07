#include "optionsstate.h"
#include "playingstate.h"
#include "optionsaction.h"
#include "backtomenuaction.h"
#include "window.h"

void OptionsState::init() {
	bHandler.addButton(new Button("Back to menu", glm::vec2(100, 100), glm::vec2(210, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new BackToMenuAction(this), "buttonback"));
}

void OptionsState::startMenu() {
	this->changeState(new MenuState());
}

void OptionsState::update(double dt) {
	glClearColor(33.0 / 255, 33.0 / 255, 33.0 / 255, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	AssetLoader::getLoader().getBigtext()->drawText("OPTIONS", glm::vec2(100, Window::getWindow().size().y - 100), glm::vec3(1, 1, 1), 1);
	bHandler.drawButtons();
	bHandler.handleButtonClicks();
}