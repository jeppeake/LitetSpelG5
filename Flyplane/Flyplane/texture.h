#pragma once
#include "lodepng.h"
#include <vector>

class Texture {
private:
	unsigned int id = 0;
	unsigned int width = 0, height = 0;
public:
	bool loadTexture(const std::string& file, int format = 0);
	void bind(unsigned int slot);
};
