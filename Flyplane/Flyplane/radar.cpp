#include "radar.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "window.h"
#include "assetloader.h"

Radar::Radar() {
	image.loadTexture("assets/textures/radar.png", 1);
	plane.loadTexture("assets/textures/radarPlane.png", 1);
	shader.create("radarVS.glsl", "radarFS.glsl");
	guiShader.create("guiVertexShader.glsl", "guiFragmentShader.glsl");
	oldAngle = 0;
	angle = 0;

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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(RadarData), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(RadarData), (GLvoid*)sizeof(glm::vec2));
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Data), (GLvoid*)(sizeof(glm::vec3) * 2));

	glBindVertexArray(0);

	float vertexbuffer[30] = {
		-1.0, -1.0, 0.0,
		 0.0,  0.0,

		 1.0,  1.0, 0.0,
		 1.0,  1.0,

		-1.0,  1.0, 0.0,
		 0.0,  1.0,

		-1.0, -1.0, 0.0,
		 0.0,  0.0,

		 1.0, -1.0, 0.0,
		 1.0,  0.0,

		 1.0,  1.0, 0.0,
		 1.0,  1.0
	};

	glGenVertexArrays(1, &guiVao);
	glGenBuffers(1, &guiVbo);
	glBindVertexArray(guiVao);
	glBindBuffer(GL_ARRAY_BUFFER, guiVbo);
	glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), vertexbuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glBindVertexArray(0);
}

void Radar::draw(float dt) {
	oldAngle = angle;
	angle += 360 * dt;
	if (angle > 360)
		angle -= 360;

	auto s = Window::getWindow().size();
	glViewport(s.x - 150, s.y - 150, 125, 125);

	guiShader.use();
	guiShader.uniform("matrix", glm::rotate(glm::mat4(1), glm::radians(angle), glm::vec3(0, 0, -1)));
	guiShader.uniform("texSampler", 0);
	glBindVertexArray(guiVao);
	glBindBuffer(GL_ARRAY_BUFFER, guiVbo);
	image.bind(0);
	//image.draw();
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//glViewport(s.x - 25 - 125 / 2 - 12, s.y - 25 - 125 / 2 - 12, 25, 25);
	glm::vec3 direction = glm::toMat3(player.orientation) * glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 up = glm::toMat3(player.orientation) * glm::vec3(1.0, 0.0, 0.0);
	//direction.y = 0;
	
	glm::mat4 view = glm::lookAt(glm::vec3(0)/*player.pos*/, glm::vec3(0, 1, 0)/**/, direction/*glm::vec3(0, 1, 0)*/);
	glm::mat4 rotate = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1, 0, 0));
	guiShader.uniform("matrix", view * rotate * glm::scale(glm::vec3(0.2, 0.2, 1)));
	plane.bind(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//shader.uniform("matrix", proj * view);
	//shader.uniform("radarAngle", angle);
	//shader.uniform("translate", player.pos);
	
	glEnable(GL_CULL_FACE);

	glViewport(s.x - 150, s.y - 150, 125, 125);

	update(dt);
	shader.use();
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, oldSize, nullptr, GL_DYNAMIC_DRAW);
	oldSize = oldBufferData.size() * sizeof(RadarData);
	//bufferData[0].x += 1.01;
	if (oldSize == 0) {
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
	}
	else {
		glBufferData(GL_ARRAY_BUFFER, oldSize, &oldBufferData[0], GL_DYNAMIC_DRAW);
	}

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glDrawArrays(GL_POINTS, 0, oldBufferData.size());

	//std::cout << oldBufferData.size() << std::endl;

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, s.x, s.y);
	bufferData.clear();
}

void Radar::setPlayer(Transform transform) {
	player = transform;
	//bufferData.push_back({ transform.pos.x, transform.pos.y, transform.pos.z, 0.0, 0.0, 1.0, 0.0 });
}

void Radar::addPlane(Transform transform) {
	float distance = glm::distance(glm::vec3(transform.pos.x, 0, transform.pos.z), glm::vec3(player.pos.x, 0, player.pos.z));

	if (distance < 10000) {
		glm::vec3 vec = glm::normalize(glm::vec3(transform.pos.x, 0, transform.pos.z) - glm::vec3(player.pos.x, 0, player.pos.z));
		float angle = glm::degrees(glm::angle(glm::vec3(0, 0, 1), vec));
		if (transform.pos.x < 0) {
			angle += 180;
		}
		glm::mat4 view = glm::lookAt(player.pos, player.pos + glm::vec3(0, -1, 0), glm::vec3(0, 0, 1)/*direction*/);
		vec = proj * view * glm::vec4(vec, 1);
		bufferData.push_back({ vec.x, vec.y, vec.z, angle });
		
		//std::cout << "x: " << vec.x << "y: " << vec.y << "z: " << vec.z << std::endl;
		//std::cout << "Distance: " <<  distance << std::endl;
	}
}

void Radar::update(float dt) {
	for (int i = oldBufferData.size() - 1; i >= 0; i--) {
		oldBufferData[i].intensity -= dt;

		if (oldBufferData[i].intensity < 0) {
			//std::cout << "Intensity: " << oldBufferData[i].intensity << std::endl;
			//std::cout << "Size: " << oldBufferData.size() << std::endl;
			//std::cout << "Index: " << i << std::endl;
			if (i >= oldBufferData.size())
				std::cout << "mohhahaha" << std::endl;
			oldBufferData.erase(oldBufferData.begin() + i);
		}
	}

	for (int i = 0; i < bufferData.size(); i++) {
		if (angle >= oldAngle) {
			if (angle >= bufferData[i].angle && oldAngle <= bufferData[i].angle) {
				oldBufferData.push_back({ bufferData[i].x, bufferData[i].y, 1.0 });
			}
		}
		else {
			if (oldAngle <= bufferData[i].angle || angle >= bufferData[i].angle) {
				oldBufferData.push_back({ bufferData[i].x, bufferData[i].y, 1.0 });
			}
		}
	}
}