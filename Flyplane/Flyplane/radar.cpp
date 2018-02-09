#include "radar.h"
#include <glm/gtc/matrix_transform.hpp>
#include "window.h"
#include "assetloader.h"

Radar::Radar() {
	image.load("assets/textures/radar.png");
	shader.create("radarVS.glsl", "radarFS.glsl");

	/*proj = glm::mat4(1.0);//glm::ortho(0, 100, 100, 0);
	proj[0][0] = 1 / 100.f;
	proj[1][1] = 0;
	proj[2][2] = 1 / 100.f;*/

	proj = glm::ortho<float>(-10000.f, 10000.f, -10000.f, 10000.f);

	//bufferData.push_back({ -100,0,0 });

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, bufferData.size() * 3 * sizeof(float), &bufferData[0], GL_STATIC_DRAW);
	oldSize = 0;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Data), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Data), (GLvoid*)sizeof(glm::vec3));

	glBindVertexArray(0);
}

void Radar::draw() {
	auto s = Window::getWindow().size();
	glViewport(s.x - 150, s.y - 150, 150, 150);

	image.bind();
	image.draw();

	shader.use();
	glm::vec3 direction = glm::toMat3(player.orientation) * glm::vec3(0.0, 0.0, 1.0);
	direction.y = 0;
	glm::mat4 view = glm::lookAt(player.pos, player.pos + glm::vec3(0, -1, 0), direction/*glm::vec3(1, 0, 0)*/);
	shader.uniform("matrix", proj * view);
	//shader.uniform("translate", player.pos);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, oldSize, nullptr, GL_DYNAMIC_DRAW);
	oldSize = bufferData.size() * sizeof(Data);
	//bufferData[0].x += 1.01;
	glBufferData(GL_ARRAY_BUFFER, oldSize, &bufferData[0], GL_DYNAMIC_DRAW);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glDrawArrays(GL_POINTS, 0, bufferData.size());

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, s.x, s.y);
	bufferData.clear();
}

void Radar::setPlayer(Transform transform) {
	player = transform;
	bufferData.push_back({ transform.pos.x, transform.pos.y, transform.pos.z, 0.0, 0.0, 1.0 });
}

void Radar::addPlane(Transform transform) {
	bufferData.push_back({ transform.pos.x, transform.pos.y, transform.pos.z, 1.0f, 0.0f, 0.0f });
}
