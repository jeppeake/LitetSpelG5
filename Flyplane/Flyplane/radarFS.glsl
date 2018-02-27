#version 420

varying float f_intensity;
varying vec3 v_color;

void main() {
	gl_FragColor = vec4(v_color, f_intensity);
}