#version 420

uniform sampler2D texSampler;
uniform sampler2D shadowMap;
uniform mat4 shadowMatrix;
in vec3 vPos;
in vec3 vNormal;
in vec2 vTex;


void main() {
	vec3 shadowCoord = (shadowMatrix * vec4(vPos, 1)).xyz;
	float depth = texture(shadowMap, vec2(shadowCoord.x, 1-shadowCoord.y)).r;
	float visibility = 1.0;
	if(depth < shadowCoord.z) {
		visibility = 0.0;
	}

	vec3 sun = vec3(0, 1, 1);
	sun = normalize(sun);
	vec3 n = normalize(vNormal);
	float result = dot(sun, n);
	result = clamp(result, 0, 1);
	vec3 color = texture(texSampler, vec2(vTex.x, 1 - vTex.y)).rgb;

	//gl_FragColor = vec4(color * result, 1) * visibility + vec4(color * 0.2, 1); //vec4(depth, depth, depth, 1);
	gl_FragColor = vec4(vec3(depth), 1);
}