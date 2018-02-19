#include "timevisualizer.h"

#include "timer.h"
#include <unordered_map>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include "shader.h"


namespace {

#define size 300
#define multi 3

	class Times {
		double times[size*multi];

		float upload[size];

		GLuint vao;
		GLuint vbo;
	public:
		Times() {
			for (int i = 0; i < size; i++)
				upload[i] = 0;


			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*size, &upload[0], GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), (GLvoid*)0);

			glBindVertexArray(0);
		}
		void draw() {
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*size, &upload[0], GL_DYNAMIC_DRAW);

			glDrawArrays(GL_LINE_STRIP, 0, size);
		}
		void add(float time) {
			for (int i = size*multi-1; i > 0; i--) {
				times[i] = times[i - 1];
			}
			times[0] = time;

			for (int i = 0; i < size; i++) {
				double max = times[i*multi];
				for (int j = 1; j < multi; j++) {
					double curr = times[i*multi + j];
					if (curr > max) {
						max = curr;
					}
				}
				upload[i] = max;
			}
		}
	};

	ShaderProgram shader;
	
	std::unordered_map<std::string, Times> times;

	std::string current;
	Timer timer;
}

void TimeVisualizer::init() {
	shader.create("timesVert.glsl", "timesFrag.glsl");
}

void TimeVisualizer::startTiming(const std::string & name) {
	current = name;
	timer.restart();
}

void TimeVisualizer::stopTiming() {
	double elapsed = timer.elapsed();
	if(current != "") {
		times[current].add(elapsed);
		current = "";
	}
}

void TimeVisualizer::draw() {
	shader.use();
	shader.uniform("size", (float)size);
	for (auto& elem : times) {
		elem.second.draw();
	}
}
