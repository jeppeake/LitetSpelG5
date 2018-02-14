#include "radar.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "window.h"
#include "assetloader.h"
#include "renderer.h"

Radar::Radar() {
	image.loadTexture("assets/textures/radar.png", 1);
	plane.loadTexture("assets/textures/radarPlane.png", 1);
	shader.create("radarVS.glsl", "radarFS.glsl");
	
	oldAngle = 0;
	angle = 0;
	oldSize = 0;

	proj = glm::ortho<float>(-10000.f, 10000.f, -10000.f, 10000.f);
	rotationMatrix = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1, 0, 0));
	scale = glm::scale(glm::vec3(0.2, 0.2, 1));

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3/*sizeof(RadarData)*/, (GLvoid*)0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4/*sizeof(RadarData)*/, (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)(sizeof(float) * 2));
	//glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (GLvoid*)(sizeof(float) * 3));

	glBindVertexArray(0);
}

void Radar::draw(float dt) {
	oldAngle = angle;
	angle += 360 * dt;
	if (angle > 360)
		angle -= 360;

	auto s = Window::getWindow().size();
	glViewport(s.x - 150, s.y - 150, 125, 125);

	Renderer::getRenderer().renderTexture(image, glm::rotate(glm::mat4(1), glm::radians((float)angle), glm::vec3(0, 0, -1)));

	glm::vec3 direction = glm::toMat3(player.orientation) * glm::vec3(0.0, 0.0, 1.0);
	//direction.y = 0;
	float directionAngle = glm::degrees(glm::angle(glm::vec3(0, 0, 1), glm::normalize(glm::vec3(direction.x, 0.f, direction.z))));
	if (direction.x > 0) {
		directionAngle = -directionAngle;
		directionAngle += 360;
	}

	
	glm::mat4 view = glm::lookAt(glm::vec3(0), glm::vec3(0, 1, 0), direction);
	Renderer::getRenderer().renderTexture(plane, view * rotationMatrix * scale);

	glDisable(GL_DEPTH_TEST);
	update(dt);
	shader.use();
	//glm::mat4 viewMatrix = glm::lookAt(player.pos, player.pos - glm::vec3(0, 1, 0), glm::vec3(0,0,1)/*direction*/);
	//glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1));
	shader.uniform("matrix", glm::rotate(glm::mat4(1), glm::radians(directionAngle), glm::vec3(0, 0, 1)));
	std::cout << directionAngle << std::endl;
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, oldSize, nullptr, GL_DYNAMIC_DRAW);
	oldSize = oldBufferData.size() * sizeof(RadarData);
	//oldSize = debugBufferData.size() * sizeof(Data);
	
	if (oldSize == 0) {
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
	}
	else {
		glBufferData(GL_ARRAY_BUFFER, oldSize, &oldBufferData[0], GL_DYNAMIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, oldSize, &debugBufferData[0], GL_DYNAMIC_DRAW);
	}

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glDrawArrays(GL_POINTS, 0, oldBufferData.size());

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, s.x, s.y);
	bufferData.clear();
}

void Radar::setPlayer(Transform transform) {
	player = transform;
}

void Radar::addPlane(Transform transform) {
	float distance = glm::distance(glm::vec3(transform.pos.x, 0, transform.pos.z), glm::vec3(player.pos.x, 0, player.pos.z));

	if (distance < 10000) {
		glm::vec3 vec = glm::normalize(glm::vec3(transform.pos.x, 0, transform.pos.z) - glm::vec3(player.pos.x, 0, player.pos.z));

		float angle = glm::degrees(glm::angle(glm::vec3(0, 0, 1), vec));
		if (vec.x > 0) {
			angle = -angle;
			angle += 360;
		}
		glm::mat4 view = glm::lookAt(player.pos, player.pos + glm::vec3(0, -1, 0), glm::vec3(0, 0, 1));
		vec = proj * view * glm::vec4(transform.pos.x, 0, transform.pos.z, 1);
		bufferData.push_back({ vec.x, vec.y, angle });
		//bufferData.push_back({ transform.pos.x, transform.pos.y, transform.pos.z, angle });
	}
}

void Radar::update(float dt) {
	for (int i = oldBufferData.size() - 1; i >= 0; i--) {
		oldBufferData[i].intensity -= dt * 3;
		//debugBufferData[i].intensity -= dt * 3;

		//if (/*old*/debugBufferData[i].intensity < 0) {
			oldBufferData.erase(oldBufferData.begin() + i);
		//}
	}
	/*debugBufferData.push_back({ 300, 200, 0, 1 });
	debugBufferData.push_back({ 3000, 200, 0, 1 });
	debugBufferData.push_back({ 300, 2000, 0, 1 });
	debugBufferData.push_back({ 3000, 2000, 0, 1 });
	debugBufferData.push_back({ player.pos.x + 2000, 0, player.pos.z + 2000, 1.0 });
	debugBufferData.push_back({ player.pos.x + 2000, 0, player.pos.z - 2000, 1.0 });
	debugBufferData.push_back({ player.pos.x - 200, 0, player.pos.z + 2000, 1.0 });
	debugBufferData.push_back({ player.pos.x - 2000, 0, player.pos.z - 2000, 1.0 });*/
	for (int i = 0; i < bufferData.size(); i++) {
		if (true || angle >= oldAngle) {
			if (true || angle >= bufferData[i].angle && oldAngle <= bufferData[i].angle) {
				oldBufferData.push_back({ bufferData[i].x, bufferData[i].y, 1.0 });
				//debugBufferData.push_back({ bufferData[i].x, 0, bufferData[i].z, 1.0 });
			}
		}
		else {
			if (oldAngle <= bufferData[i].angle || angle >= bufferData[i].angle) {
				debugBufferData.push_back({ bufferData[i].x, 0, bufferData[i].z, 1.0 });
			}
		}
	}
}