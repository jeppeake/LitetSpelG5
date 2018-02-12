#version 420

varying vec3 f_color;
varying float f_angle;

void main() {
	float intensity = 0.6;
	if (f_angle == 15.0)
		intensity = 0.3;

	gl_FragColor = vec4(intensity * f_color, 1);
}