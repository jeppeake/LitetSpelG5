#include "shader.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

GLuint compileShader(GLenum type, const std::string& name)
{
	GLuint shader = glCreateShader(type);

	std::string path = name;
	std::ifstream shaderFile(path);
	std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	if (!shaderFile.is_open())
	{
		std::cout << "ERROR: Could not open file: " << path << "\n";
		system("pause");
		exit(1);
	}

	shaderFile.close();
	const char* shaderTextPtr = shaderText.c_str();

	glShaderSource(shader, 1, &shaderTextPtr, nullptr);
	glCompileShader(shader);

	// Check for compile error
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint log_size = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
		std::vector<GLchar> error(log_size);
		glGetShaderInfoLog(shader, log_size, &log_size, &error[0]);
		std::string errorstr{ &error[0] };

		std::cout << errorstr << "\n";

		glDeleteShader(shader);
		system("pause");
		exit(1);
	}





	return shader;
}

ShaderProgram::ShaderProgram()
{
	vs = 0;
	gs = 0;
	fs = 0;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteShader(vs);
	glDeleteShader(gs);
	glDeleteShader(fs);
	glDeleteProgram(id);
}

void ShaderProgram::create(const std::string & vert, const std::string & geom, const std::string & frag)
{
	vs = compileShader(GL_VERTEX_SHADER, vert);
	gs = compileShader(GL_GEOMETRY_SHADER, geom);
	fs = compileShader(GL_FRAGMENT_SHADER, frag);

	id = glCreateProgram();
	glAttachShader(id, fs);
	glAttachShader(id, gs);
	glAttachShader(id, vs);
	glLinkProgram(id);
}

void ShaderProgram::create(const std::string & vert, const std::string & frag)
{
	GLuint vs = compileShader(GL_VERTEX_SHADER, vert);
	GLuint fs = compileShader(GL_FRAGMENT_SHADER, frag);

	id = glCreateProgram();
	glAttachShader(id, vs);
	glAttachShader(id, fs);
	glLinkProgram(id);
	
	GLint success = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint log_size = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_size);
		std::vector<GLchar> error(log_size);
		glGetShaderInfoLog(id, log_size, &log_size, &error[0]);
		std::string errorstr{ &error[0] };

		std::cout << errorstr << "\n";

		glDeleteProgram(id);
		system("pause");
		exit(1);
	}

}

void ShaderProgram::use()
{
	glUseProgram(id);
}


GLuint ShaderProgram::findUniformLocation(const std::string & name)
{
	auto it = uniform_locations.find(name);
	GLuint uniform_location;
	if (it == uniform_locations.end())
	{
		uniform_location = glGetUniformLocation(id, name.c_str());
		uniform_locations[name] = uniform_location;
	}
	else
		uniform_location = it->second;
	return uniform_location;
}


void ShaderProgram::uniformv(const std::string & name, GLuint count, const glm::mat4* matrices)
{
	glUniformMatrix4fv(findUniformLocation(name), count, GL_FALSE, (GLfloat*)matrices);
}
void ShaderProgram::uniform(const std::string & name, const glm::mat4& matrix)
{
	uniformv(name, 1, &matrix);
}


void ShaderProgram::uniformv(const std::string & name, GLuint count, const GLfloat* values)
{
	glUniform1fv(findUniformLocation(name), count, (GLfloat*)values);
}
void ShaderProgram::uniform(const std::string & name, GLfloat value)
{
	uniformv(name, 1, &value);
}


void ShaderProgram::uniformv(const std::string & name, GLuint count, const glm::vec2* vectors)
{
	glUniform2fv(findUniformLocation(name), count, (GLfloat*)vectors);
}
void ShaderProgram::uniform(const std::string & name, const glm::vec2& vector)
{
	uniformv(name, 1, &vector);
}


void ShaderProgram::uniformv(const std::string & name, GLuint count, const glm::vec3* vectors)
{
	glUniform3fv(findUniformLocation(name), count, (GLfloat*)vectors);
}
void ShaderProgram::uniform(const std::string & name, const glm::vec3& vector)
{
	uniformv(name, 1, &vector);
}

void ShaderProgram::uniformv(const std::string & name, GLuint count, const glm::vec4* vectors)
{
	glUniform4fv(findUniformLocation(name), count, (GLfloat*)vectors);
}
void ShaderProgram::uniform(const std::string & name, const glm::vec4& vector)
{
	uniformv(name, 1, &vector);
}




void ShaderProgram::uniformv(const std::string & name, GLuint count, const GLint* values)
{
	glUniform1iv(findUniformLocation(name), count, values);
}
void ShaderProgram::uniform(const std::string & name, GLint value)
{
	uniformv(name, 1, &value);
}
