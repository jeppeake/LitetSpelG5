#include <GL\glew.h>
#include "window.h"
#include "enginestate.h"
#include "playingstate.h"
#include "renderer.h"
#include "model.h"
#include "input.h"

int main(void)
{	
	Window::getWindow().open(1280, 720);
	//w.open();
	Renderer r;
	Model m;
	m.load("assets/MIG-212A.fbx");

	Heightmap map("cloude.png", "terraintex.png");

	EngineState* engine = new PlayingState();
	engine->init();

	while (!Window::getWindow().shouldClose()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		r.Render(m);
		r.Render(map);
		engine->update();

		EngineState* new_state = engine->newState();
		if (new_state) {
			delete engine;
			engine = new_state;
			engine->init();
		}
		if (Input::isKeyPressed(GLFW_KEY_A))
		{
			std::cout << "Shiiieeet" << std::endl;
		}
		Window::getWindow().update();
	}
	
	return 0;
}