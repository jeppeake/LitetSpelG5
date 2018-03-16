#version 420
layout (location = 0) in vec2 pos;
layout (location = 1) in float intensity;
layout (location = 2) in vec3 color;

uniform mat4 matrix;

varying float f_intensity;
varying vec3 v_color;

void main() {
	gl_Position = matrix * vec4(pos, 0, 1);
	gl_PointSize = 5;
	f_intensity = intensity;
	v_color = color;
}