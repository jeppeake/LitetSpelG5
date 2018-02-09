#pragma once

#include <GL\glew.h>
#include <vector>
#include <glm\glm.hpp>

struct Marker {
	glm::vec3 pos;
	float scale;
};

class EnemyMarker {
private:
	std::vector<Marker> markers;
	GLuint vbo = 0;
	GLuint vao = 0;
	/*float vertexdata[15] = { -5.0f, 5.0f, 0.0f,
							 -5.0f, -5.0f, 0.0f,
							  5.0f, -5.0f, 0.0f,
							  5.0f,  5.0f, 0.0f,
						     -5.0f, 5.0f, 0.0f };*/
	float vertexdata[3] = { 0.0f, 0.0f, 0.0f };
public:
	EnemyMarker();
	~EnemyMarker();
	void bind();
	void addPosition(glm::vec3 pos, float scale);
	std::vector<Marker>& getMarkers();
	void clear();
};