#version 420

//varying vec3 f_color;
//varying float f_angle;
varying float f_intensity;

//uniform float radarAngle;

void main() {
	/*float intensity = 1.0;

	float angle = radarAngle - f_angle;
	if (angle < 0)
		angle += 360;

	intensity = (360 - angle) / 360;*/

	gl_FragColor = vec4(f_intensity * vec3(1, 0, 0), 1);
}