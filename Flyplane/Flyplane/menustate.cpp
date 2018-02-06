#include "menustate.h"
#include "input.h"
#include "playingstate.h"
#include "assetloader.h"

MenuState::~MenuState() {
}

void MenuState::init() {
}

void MenuState::update(double dt) {
	glClearColor(1, 1, 1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	AssetLoader::getLoader().getText()->drawText("Press enter to start!", glm::vec2(400, 500), glm::vec3(1, 0, 0), 1);

	if (Input::isKeyPressed(GLFW_KEY_ENTER)) {
		this->changeState(new PlayingState());
	}

}
