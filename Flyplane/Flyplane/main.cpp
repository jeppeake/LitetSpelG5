#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h> 
#include <glm\glm.hpp>
#include <entityx\entityx.h>

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		//system("pause");
		
	}


	struct Position {
		Position(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

		float x, y, z;
	};

	struct WeaponStats {
		//projectile model, sound, range?, ammo, speed, 
		WeaponStats(unsigned int ammo, unsigned int range, float speed, float falloff) : ammo(ammo), range(range), speed(speed), falloff(falloff) {}

		unsigned int ammo, range;
		float speed, falloff;
	};

	Assimp::Importer importer;

	entityx::EntityX ex;

	entityx::Entity entity = ex.entities.create();

	entityx::Entity testWep = ex.entities.create();

	testWep.assign<WeaponStats>(10, 100, 100.0f, 0.01f);
	testWep.assign<Position>(0.0f, 0.0f, 0.0f);




	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}