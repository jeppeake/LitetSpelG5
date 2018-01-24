#include <GL\glew.h>
#include "window.h"
#include "enginestate.h"
#include "playingstate.h"
#include "renderer.h"
#include "model.h"

int main(void)
{
	Window w;
	
	w.open(1280, 720);
	Renderer r;
	Model m;
	m.load("assets/MIG-212A.fbx");

	Heightmap map("korven.png");

	EngineState* engine = new PlayingState();
	engine->init();

	while (!w.shouldClose()) {
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

		w.update();
	}
	
	return 0;
}