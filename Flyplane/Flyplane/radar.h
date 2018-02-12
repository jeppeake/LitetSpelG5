#pragma once
#include <GL\glew.h>
#include "shader.h"
#include "transform.h"
#include <vector>
#include <glm/glm.hpp>
#include "texture.h"

struct Data {
	float x, y, z;
	float r, g, b;
};

class Radar {
private:
	Texture image;
	ShaderProgram shader;
	ShaderProgram guiShader;
	GLuint vao, vbo;
	GLuint guiVao, guiVbo;
	Transform player;
	std::vector<Data> bufferData;
	glm::mat4 proj;
	int oldSize;
	float oldAngle;
	float angle;
public:
	Radar();
	void draw(float dt);
	void setPlayer(Transform transform);
	void addPlane(Transform transform);
};