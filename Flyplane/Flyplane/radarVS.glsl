#version 420
layout (location = 0) in vec4 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in float angle;

varying vec3 f_color;
varying float f_angle;

uniform mat4 matrix;
//uniform vec3 translate;

void main() {
	gl_Position = matrix * pos;
	//gl_Position = (pos - vec4(translate, 0));
	gl_Position.z = 0;
	//gl_Position = vec4(0,0,0,1);
	gl_PointSize = 5;
	f_color = color;
	f_angle = angle;
}