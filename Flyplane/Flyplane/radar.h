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
	float intensity;
};

struct NewPlane {
	float x, y, z;
	float angle;
};

struct RadarData {
	float x, y;
	float intensity;
};


class Radar {
private:
	Texture image;
	Texture plane;
	ShaderProgram shader;
	ShaderProgram guiShader;
	GLuint vao, vbo;
	GLuint guiVao, guiVbo;
	Transform player;
	std::vector<RadarData> oldBufferData;
	std::vector<NewPlane> bufferData;
	glm::mat4 proj;
	int oldSize;
	float oldAngle;
	float angle;

	void update(float dt);
public:
	Radar();
	void draw(float dt);
	void setPlayer(Transform transform);
	void addPlane(Transform transform);
};