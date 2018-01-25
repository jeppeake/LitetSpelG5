#include <GL\glew.h>
#include <thread>
#include "window.h"
#include "enginestate.h"
#include "playingstate.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "transform.h"
int main(void)
{	
	srand(time(NULL));
	Window::getWindow().open(1280, 720);
	//w.open();
	Input::initialize();
	Model m;
	m.load("assets/MIG-212A.fbx");

	Heightmap map("assets/textures/cloude.png", "assets/textures/bog.png");

	EngineState* engine = new PlayingState();
	engine->init();

	Timer deltatime;

	auto t = Transform();

	while (!Window::getWindow().shouldClose()) {
		double dt = deltatime.restart();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		t.pos.x += 0.03;

		t.orientation += glm::quat(0.01, 0.01, -0.01, 0)*t.orientation;
		t.orientation = normalize(t.orientation);

		Renderer::getRenderer().Render(m, t);
		Renderer::getRenderer().Render(map);
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