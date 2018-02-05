#pragma once
#include <map>
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include "shader.h"
struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};
class Text
{
private:
	ShaderProgram p;
	std::map<GLchar, Character> Characters;
	glm::mat4 projection;
	GLuint VAO, VBO;
public:
	Text(const std::string &Shipath, unsigned size);
	void drawText(const std::string &text, glm::vec2 pos, glm::vec3 col, GLfloat scale);
	~Text();
};

