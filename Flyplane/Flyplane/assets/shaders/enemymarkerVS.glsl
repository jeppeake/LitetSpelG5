#version 420
layout(location = 0) in vec3 pos;
//layout(location = 1) in vec2 tex;
uniform mat4 modelMatrix;
uniform vec3 transform;

out vec3 Pos;

//varying vec2 Tex;
void main() {

	//Tex = tex;
	//vec4 position = modelMatrix * vec4(pos, 1);
	//gl_Position = aspectMatrix * position;

	Pos = transform;
}