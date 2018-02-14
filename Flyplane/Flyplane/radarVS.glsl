#version 420
layout (location = 0) in vec2 pos;
layout (location = 1) in float intensity;


varying float f_intensity;

void main() {
	gl_Position = vec4(pos, 0, 1);
	gl_PointSize = 5;
	f_intensity = intensity;
}