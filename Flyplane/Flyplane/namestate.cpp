#include "namestate.h"
#include "loadingstate.h"


void NameState::checkForInput() {
	int offset = 'a' - 'A';
	if (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
		offset = 0;
	}
	for (int i = 0; i < 25; i++) {
		if (Input::isKeyPressed(GLFW_KEY_A + i) && name.size() < 17) {
			name += GLFW_KEY_A + i + offset;
		}
	}
	if (Input::isKeyPressed(GLFW_KEY_BACKSPACE) && name.size() > 0)
		name.pop_back();
}

void NameState::init() {
	
}

void NameState::update(double dt) {
	glClearColor(33.0 / 255, 33.0 / 255, 33.0 / 255, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	checkForInput();
	glm::vec2 pos;
	pos.x = 500;
	pos.y = 600;
	AssetLoader::getLoader().getMenutext()->drawText("Enter name!", pos, glm::vec3(1, 1, 1), 1.0);
	pos.x = 500;
	pos.y = 500;
	AssetLoader::getLoader().getText()->drawText(name, pos, glm::vec3(1, 1, 1), 0.7);
	if (Input::isKeyPressed(GLFW_KEY_ENTER) && name.size() > 0) {
		this->changeState(new LoadingState(name));
	}
}

void NameState::startMenu() {

}
