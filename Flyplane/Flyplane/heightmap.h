#pragma once
#include <vector>
class Heightmap {
private:
	std::vector<unsigned char> heightmap;
	unsigned int width, height;
	std::vector<float> vertices;
public:
	Heightmap(const std::string &file);
};