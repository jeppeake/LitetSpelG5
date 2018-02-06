#include "menustate.h"
#include "input.h"
#include "playingstate.h"

MenuState::~MenuState() {
}

void MenuState::init() {
}

void MenuState::update(double dt) {
	glClearColor(1, 1, 1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (Input::isKeyPressed(GLFW_KEY_ENTER)) {
		this->changeState(new PlayingState());
	}

}
