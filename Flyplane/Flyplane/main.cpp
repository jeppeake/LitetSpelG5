#include <GL\glew.h>
#include <thread>
#include "window.h"
#include "enginestate.h"
#include "playingstate.h"
#include "menustate.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "transform.h"
#include <sfml\Audio.hpp>
#include "timevisualizer.h"
#include "highscore.h"
#include "globaltimer.h"

int main(void)
{	
	sf::Listener::setGlobalVolume(30);

	srand(time(NULL));
	Window::getWindow().open(1280, 720);
	//w.open();
	Input::initialize();

	EngineState* engine = new MenuState();
	engine->init();

	TimeVisualizer::init();

	Timer deltatime;
	while (!Window::getWindow().shouldClose()) {
		double dt = deltatime.restart();

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//TimeVisualizer::startTiming("everything");
		GlobalTimer::update();

		Renderer::getRenderer().update(dt);
		engine->update(dt);

		//TimeVisualizer::stopTiming();
		//TimeVisualizer::draw();

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