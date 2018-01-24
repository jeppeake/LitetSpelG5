uniform sampler2D texSampler;
in vec3 vPos;
in vec3 vNormal;
in vec2 vTex;


void main() {
	vec3 sun = vec3(0, 1, 1);
	normalize(sun);
	normalize(vNormal);
	float result = dot(sun, vNormal);
	result = clamp(result, 0, 1);
	vec3 color = texture(texSampler, vec2(vTex.x, 1 - vTex.y)).rgb;
	gl_FragColor = vec4(color * result, 1) + vec4(color * 0.2, 1);
}