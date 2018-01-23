#pragma once
#include <vector>
class Heightmap {
private:
	std::vector<unsigned char> heightmap;
	unsigned int width, height;
	std::vector<float> vertices;
	std::vector<float> indices;
	unsigned int hScale = 10;
public:
	Heightmap(const std::string &file);
};