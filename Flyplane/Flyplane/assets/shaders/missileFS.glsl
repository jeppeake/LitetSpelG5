#version 420
uniform sampler2D texSampler;
uniform sampler2D Sampler;

varying vec3 Normal;
varying vec2 Tex;


void main() {
	vec3 color = texture(Sampler, vec2(Tex.x, 1 - Tex.y)).rgb;


	vec3 sun = normalize(vec3(0, 0, 1));
	vec3 n = normalize(Normal);

	float diffuse = dot(sun, n);
	diffuse = clamp(diffuse, 0, 1);

	
	gl_FragColor = vec4(color * diffuse, 1);
	
}