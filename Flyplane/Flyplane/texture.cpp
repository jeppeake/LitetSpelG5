#include "texture.h"
#include <GL/glew.h>
#include <iostream>

bool Texture::loadTexture(const std::string& file, int format) {
	std::vector<unsigned char> image;
	unsigned error = lodepng::decode(image, width, height, file);
	if (error != 0) {
		std::cout << "[ERROR] Could not load texture: " << file << "\n";
		return false;
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (format) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	}

	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	}
	

	glGenerateMipmap(GL_TEXTURE_2D);

	image.clear();
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}



void Texture::bind(unsigned int slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

unsigned Texture::getID()
{
	return id;
}
