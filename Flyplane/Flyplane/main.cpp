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
#include "musicmanager.h"
#include <Windows.h>

int main(void)
{	
	
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
	std::cout.setstate(std::ios_base::failbit);

	srand(time(NULL));
	sf::Listener::setGlobalVolume(50);
	MusicManager::getMusicManager().playMenuMusic();

	Window::getWindow().open(1280, 720);
	//w.open();
	Input::initialize();

	EngineState* engine = new MenuState();
	engine->init();

	//TimeVisualizer::init();

	Timer deltatime;
	while (!Window::getWindow().shouldClose()) {
		double dt = deltatime.restart();
		double dtMax = 4.0 / 60.0;
		if (dt >= dtMax) {
			dt = dtMax;
		}
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
		MusicManager::getMusicManager().update();
	}
	
	return 0;
}