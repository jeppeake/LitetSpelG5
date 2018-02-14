#pragma once
#include <GL\glew.h>
#include "shader.h"
#include "transform.h"
#include <vector>
#include <glm/glm.hpp>
#include "texture.h"

struct NewPlane {
	float x, y;
	float angle;
};

struct RadarData {
	float x, y;
	float intensity;
};

/*
struct Data {
	float x, y, z;
	float intensity;
};
*/


class Radar {
private:
	Texture image;
	Texture plane;
	Texture radarRay;
	ShaderProgram shader;
	GLuint vao, vbo;
	Transform player;
	std::vector<RadarData> oldBufferData;
	//std::vector<Data> debugBufferData;
	std::vector<NewPlane> bufferData;
	glm::mat4 proj;
	glm::mat4 rotationMatrix;
	glm::mat4 scale;
	int oldSize;
	double oldAngle;
	double angle;

	void update(float dt);
public:
	Radar();
	void draw(float dt);
	void setPlayer(Transform transform);
	void addPlane(Transform transform);
};