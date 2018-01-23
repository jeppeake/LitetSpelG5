#include "renderer.h"
#include <fstream>
#include <iostream>

using namespace std;

Renderer::Renderer() {
	float vertices[9] = {-1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0};
	
	glGenBuffers(1, &this->vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	ifstream shaderFile("vertexShader.glsl");
	std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();
	const char* shaderTextPtr = shaderText.c_str();

	this->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->vertex_shader, 1, &shaderTextPtr, NULL);
	glCompileShader(this->vertex_shader);

	ifstream shaderFile1("fragmentShader.glsl");
	std::string shaderText1((std::istreambuf_iterator<char>(shaderFile1)), std::istreambuf_iterator<char>());
	shaderFile.close();
	const char* shaderTextPtr1 = shaderText1.c_str();

	this->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->fragment_shader, 1, &shaderTextPtr1, NULL);
	glCompileShader(this->fragment_shader);

	/*int result;
	glGetShaderiv(this->fragment_shader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(this->fragment_shader, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(this->fragment_shader, length, &length, message);
		cout << message << endl;
	}*/

	this->program = glCreateProgram();
	glAttachShader(this->program, this->vertex_shader);
	glAttachShader(this->program, this->fragment_shader);
	glLinkProgram(this->program);

	this->vertex_pos = glGetAttribLocation(this->program, "pos");
	//this->vertex_tex = glGetUniformLocation(this->program, "tex");

	glEnableVertexAttribArray(this->vertex_pos);
	glVertexAttribPointer(this->vertex_pos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*) (sizeof(float) * 3));

	//glEnableVertexAttribArray(this->vertex_tex);
	//glVertexAttribPointer(this->vertex_tex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*) (sizeof(float) * 2));
}

Renderer::~Renderer() {

}

void Renderer::Render() {
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
