#include <GL\glew.h>

#include "window.h"

int main(void)
{
	Window w;
	w.open(1280, 720);

	while (!w.shouldClose()) {

		w.update();
	}
	
	return 0;
}