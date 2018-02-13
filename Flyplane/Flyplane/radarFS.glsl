#version 420

//varying vec3 f_color;
//varying float f_angle;
varying float f_intensity;

//uniform float radarAngle;

void main() {
	vec4 color = vec4(1, 0, 0, 1);

	if (f_intensity < 90)
		color = vec4(0, 1, 0, 1);
	else if (f_intensity < 180)
		color = vec4(0, 0, 1, 1);
	else if (f_intensity < 270)
		color = vec4(1, 1, 1, 1);

	gl_FragColor = color;
	//gl_FragColor = vec4(vec3(1, 0, 0), f_intensity);
}