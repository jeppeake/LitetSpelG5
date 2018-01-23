#include <GL\glew.h>
#include "window.h"
#include "enginestate.h"
#include "playingstate.h"

int main(void)
{
	Window w;
	w.open(1280, 720);


	EngineState* engine = new PlayingState();
	engine->init();

	while (!w.shouldClose()) {
		glClear(GL_COLOR_BUFFER_BIT);

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