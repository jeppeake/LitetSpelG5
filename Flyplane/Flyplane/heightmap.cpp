#include "heightmap.h"
#include "lodepng.h"
#include <iostream>
Heightmap::Heightmap(const std::string &file) {
	std::vector<unsigned char> img;
	unsigned error = lodepng::decode(img, width, height, file);
	if (error != 0) {
		std::cout << "[ERROR] Failed to load heightmap." << "\n";
	}

	int vcount = 0;
	for (int i = 0; i < width*height * 4; i+=4) {
		//std::cout << (unsigned int)img[i] << " " << (unsigned int)img[i+1] << " " << (unsigned int)img[i+2] << " " << (unsigned int)img[i + 3] << "\n";
		int sumColors = (255-(unsigned int)img[i]) + (255-(unsigned int)img[i + 1]) + (255-(unsigned int)img[i + 2]) + (255-(unsigned int)img[i + 3]);
		std::cout << "Color intesnity:" << sumColors << "\n";
		vertices.push_back(sumColors * hScale);
		vcount++;
	}
	
	for (int i = 0; i < width*height-width; i+=1) {
		indices.push_back(i);
		indices.push_back(i + width);
		std::cout << i << " " << i + width << " ";
	}


}