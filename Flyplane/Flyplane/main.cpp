#include <GL\glew.h>
#include <thread>
#include "window.h"
#include "enginestate.h"
#include "playingstate.h"
#include "renderer.h"
#include "model.h"
#include "input.h"

int main(void)
{	
	srand(time(NULL));
	Window::getWindow().open(1280, 720);
	//w.open();
	Renderer r;
	Input::initialize();
	Model m;
	m.load("assets/MIG-212A.fbx");

	Heightmap map("cloude.png", "bog.png");

	EngineState* engine = new PlayingState();
	engine->init();

	Timer deltatime;

	while (!Window::getWindow().shouldClose()) {
		double dt = deltatime.restart();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		r.Render(m);
		r.Render(map);
		r.update(dt);
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