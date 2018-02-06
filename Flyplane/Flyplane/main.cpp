#include <GL\glew.h>
#include <thread>
#include "window.h"
#include "enginestate.h"
#include "playingstate.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "transform.h"
#include <sfml\Audio.hpp>
#include "endlessmode.h"

int main(void)
{	
	sf::Listener::setGlobalVolume(30);
	//sf::Listener::setPosition(0,1,0);
	//sf::Listener::setDirection(1, 0, 0);

	srand(time(NULL));
	Window::getWindow().open(1280, 720);
	//w.open();
	Input::initialize();

	EngineState* engine = new PlayingState();//EndlessMode();
	engine->init();

	Timer deltatime;

	while (!Window::getWindow().shouldClose()) {
		double dt = deltatime.restart();

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Renderer::getRenderer().update(dt);
		engine->update(dt);

		EngineState* new_state = engine->newState();
		if (new_state) {
			delete engine;
			engine = new_state;
			engine->init();
		}
		Input::reset();
		Window::getWindow().update();
	}
	
	return 0;
}