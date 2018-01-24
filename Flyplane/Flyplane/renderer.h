#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <glm\glm.hpp>
#include "model.h"
#include "camera.h"

class Renderer {
private:
	ShaderProgram shader;
	glm::mat4x4 matrix;
	Camera camera;
	float tal = 0;
public:
	Renderer();
	~Renderer();
	void Render(Model &model);
};