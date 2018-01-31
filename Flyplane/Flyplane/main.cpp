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
int main(void)
{	
	sf::Listener::setGlobalVolume(50);
	sf::Listener::setPosition(0,0,0);
	sf::Listener::setDirection(1, 0, 0);
	sf::SoundBuffer buffer;
	//buffer.loadFromSamples(&samples[0], samples.size(), 1, 44100);
	if (!buffer.loadFromFile("assets/Sound/airplane-takeoff.wav"))
		std::cout << "sound coludnt load" << std::endl;
	sf::Sound sound;
	float pos = 10;
	sound.setPosition(-10, 0, 0);
	sound.setMinDistance(10);
	sound.setAttenuation(10);
	sound.setBuffer(buffer);
	sound.setLoop(true);
	sound.play();
	sound.setVolume(50);

	srand(time(NULL));
	Window::getWindow().open(1280, 720);
	//w.open();
	Input::initialize();

	EngineState* engine = new PlayingState();
	engine->init();

	Timer deltatime;

	while (!Window::getWindow().shouldClose()) {
		pos += 0.01;
		sound.setPosition(pos, 0, 0);
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