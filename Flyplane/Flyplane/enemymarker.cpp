#include "enemymarker.h"

EnemyMarker::EnemyMarker() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, vertexdata, GL_STATIC_DRAW);


	unsigned int stride = 3 * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);

	glBindVertexArray(0);
	
	glLineWidth(2.0f);
}

EnemyMarker::~EnemyMarker() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void EnemyMarker::bind() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void EnemyMarker::addPosition(glm::vec3 pos, float scale) {
	this->markers.push_back({ pos, scale });
}

std::vector<Marker>& EnemyMarker::getMarkers() {
	return this->markers;
}

void EnemyMarker::clear() {
	markers.clear();
}
