#include "gameoverstate.h"
#include "window.h"
#include "playingstate.h"

GameOverState::~GameOverState()
{
}

void GameOverState::init()
{
}

void GameOverState::update(double dt) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	auto s = Window::getWindow().size();
	glViewport(0, 0, s.x, s.y);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glm::vec2 pos = Window::getWindow().size();
	pos.x = pos.x / 2 - 20;
	pos.y -= 50;
	AssetLoader::getLoader().getText()->drawText("HIGH SCORES", pos, glm::vec3(1, 0, 0), 0.4);

	if (Input::isKeyPressed(GLFW_KEY_F5)) {
		this->changeState(new PlayingState());
	}
}
