uniform sampler2D texSampler;
varying vec3 Pos;
varying vec3 Normal;
varying vec2 Tex;


void main() {
	vec3 sun = vec3(0, 1, 1);
	normalize(sun);
	normalize(Normal);
	float result = dot(sun, Normal);
	result = clamp(result, 0, 1);
	gl_FragColor = vec4(texture(texSampler, Tex).rgb, 1);
}