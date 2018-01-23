#pragma once
#include "lodepng.h"
#include <vector>

class Texture {
private:
	unsigned int id;
	unsigned int width, height;
public:
	bool loadTexture(const std::string& file);
	void bind(unsigned int slot);
};
