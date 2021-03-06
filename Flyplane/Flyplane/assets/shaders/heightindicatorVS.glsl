#version 420
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

uniform vec2 value;
varying vec2 Tex;

void main() {
	Tex = tex + value;
	gl_Position = vec4(pos, 1);
}