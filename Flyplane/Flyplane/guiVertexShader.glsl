#version 420
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
uniform mat3 modelMatrix;

varying vec2 Tex;
void main() {
	Tex = tex;
	gl_Position = vec4(modelMatrix * pos, 1);
}