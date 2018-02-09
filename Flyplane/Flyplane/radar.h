#pragma once
#include <GL\glew.h>
#include "shader.h"
#include "transform.h"
#include <vector>
#include <glm/glm.hpp>
#include "fullscreenimage.h"

struct Data {
	float x, y, z;
	float r, g, b;
};

class Radar {
private:
	FullscreenImage image;
	ShaderProgram shader;
	GLuint vao, vbo;
	Transform player;
	std::vector<Data> bufferData;
	glm::mat4 proj;
	int oldSize;
public:
	Radar();
	void draw();
	void setPlayer(Transform transform);
	void addPlane(Transform transform);
};