#version 420

varying float f_intensity;

void main() {
	gl_FragColor = vec4(vec3(1, 0, 0), f_intensity);
}