#version 420

uniform sampler2D texSampler;
uniform sampler2D shadowMap;
uniform mat4 shadowMatrix;
in vec3 vPos;
in vec3 vNormal;
in vec2 vTex;


void main() {
	vec3 shadowCoord = (shadowMatrix * vec4(vPos, 1)).xyz;
	float depth = texture(shadowMap, shadowCoord.xy).z;
	float visibility = 1.0;
	if(depth < shadowCoord.z) {
		visibility = 0.5;
	}

	vec3 sun = vec3(0, 1, 1);
	normalize(sun);
	normalize(vNormal);
	float result = dot(sun, vNormal);
	result = clamp(result, 0, 1);
	vec3 color = texture(texSampler, vec2(vTex.x, 1 - vTex.y)).rgb;
	gl_FragColor = vec4(color * result, 1) * visibility + vec4(color * 0.2, 1);
}