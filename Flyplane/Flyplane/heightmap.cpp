#include "heightmap.h"
#include "lodepng.h"
#include <iostream>
Heightmap::Heightmap(const std::string &file) {
	std::vector<unsigned char> img;
	unsigned error = lodepng::decode(img, width, height, file);
	if (error != 0) {
		std::cout << "[ERROR] Failed to load heightmap." << "\n";
	}

	for (int w = 0; w < width; w++) {
		for (int h = 0; h < height; h++) {
			std::cout << img[w + h*width + 0] << img[w + h*width + 1] << img[w + h*width + 2] << img[w + h*width + 3] << "/n";
		}
	}
}