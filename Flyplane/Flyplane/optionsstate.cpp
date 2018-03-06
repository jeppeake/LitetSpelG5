#include "optionsstate.h"
#include "playingstate.h"
#include "optionsaction.h"
#include "backtomenuaction.h"
#include "window.h"

void OptionsState::init() {
	glm::vec2 s = Window::getWindow().size();
	bHandler.addButton(new Button("Back to menu", glm::vec2(s.x - 200, s.y - 100), glm::vec2(200, 40), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new BackToMenuAction(this), "buttonback"));
	fi.load("assets/textures/tutorial.png");
}

void OptionsState::startMenu() {
	this->changeState(new MenuState());
}

void OptionsState::update(double dt) {
	glClearColor(33.0 / 255, 33.0 / 255, 33.0 / 255, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	fi.bind();
	fi.draw();
	glClear(GL_DEPTH_BUFFER_BIT);
	bHandler.drawButtons();
	bHandler.handleButtonClicks();
}