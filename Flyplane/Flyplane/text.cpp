#include "text.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include "window.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
Text::Text(const std::string &path, unsigned size) {
	p.create("textVert.glsl", "textFrag.glsl");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cerr << "failed to init \n";
		return;
	}
	FT_Face face;
	if (FT_New_Face(ft, path.c_str(), 0, &face) || !face) {
		std::cerr << "failed to load font \n";
		return;
	}
	FT_Set_Pixel_Sizes(face, 0, size);
	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Error Error = FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "Failed to load Glyph: " << (unsigned)c << " Error: " << Error << "\n";
			continue;
		}
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	projection = glm::ortho(0.0, (double)Window::getWindow().size().x, 0.0, (double)Window::getWindow().size().y);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(p.getId(), "vertex"));
	glVertexAttribPointer(glGetAttribLocation(p.getId(), "vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::drawText(const std::string & text, glm::vec2 pos, glm::vec3 col, GLfloat scale) {
	setUniforms(col);


	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		drawCharacter(c, pos, scale);
	}
	unbind();
}

void Text::unbind() {
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Text::drawCharacter(std::string::const_iterator &c, glm::vec2 &pos, const GLfloat &scale) {
	Character ch = Characters[*c];

	GLfloat xpos = pos.x + ch.Bearing.x * scale;
	GLfloat ypos = pos.y - (ch.Size.y - ch.Bearing.y) * scale;

	GLfloat w = ch.Size.x * scale;
	GLfloat h = ch.Size.y * scale;
	GLfloat vertices[6][4] = {
		{ xpos,     ypos + h,   0.0, 0.0 },
	{ xpos,     ypos,       0.0, 1.0 },
	{ xpos + w, ypos,       1.0, 1.0 },

	{ xpos,     ypos + h,   0.0, 0.0 },
	{ xpos + w, ypos,       1.0, 1.0 },
	{ xpos + w, ypos + h,   1.0, 0.0 }
	};
	glBindTexture(GL_TEXTURE_2D, ch.TextureID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, vertices, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	pos.x += (ch.Advance >> 6) * scale;
}
void Text::setUniforms(glm::vec3 &col) {

	p.use();
	projection = glm::ortho(0.0, (double)Window::getWindow().size().x, 0.0, (double)Window::getWindow().size().y);
	p.uniform("textColor", col);
	p.uniform("projection", projection);
	p.uniform("text", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
}
Text::~Text() {
	for (auto &character : Characters)
	{
		glDeleteTextures(1, &character.second.TextureID);
	}
}
