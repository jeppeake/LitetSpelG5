#include "menustate.h"
#include "input.h"
#include "playingstate.h"

MenuState::~MenuState() {
}

void MenuState::init() {
}

void MenuState::update(double dt) {
	if (Input::isKeyPressed(GLFW_KEY_ENTER)) {
		this->changeState(new PlayingState());
	}
}
