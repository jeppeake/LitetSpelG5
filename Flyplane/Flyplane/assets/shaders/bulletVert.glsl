#version 430 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 orientation;



out vec3 Pos;
out vec4 Orientation;



void main() {
	Pos = pos;
	Orientation = orientation;
}