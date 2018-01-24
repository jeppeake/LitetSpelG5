#include "texture.h"
#include <GL/glew.h>
#include <iostream>

bool Texture::loadTexture(const std::string& file) {
	std::vector<unsigned char> image;
	unsigned error = lodepng::decode(image, width, height, file);
	if (error != 0) {
		std::cout << "[ERROR] Could not load texture: " << file << "\n";
		return false;
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	image.clear();
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void Texture::bind(unsigned int slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}