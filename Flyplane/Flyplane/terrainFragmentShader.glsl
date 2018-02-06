#version 420

uniform sampler2D texSampler;
uniform sampler2D shadowMap;
uniform mat4 shadowMatrix;
in vec3 vPos;
in vec3 vNormal;
//in vec2 vTex;


void main() {
	vec3 shadowCoord = (shadowMatrix * vec4(vPos, 1)).xyz;
	float depth = texture(shadowMap, shadowCoord.xy).r;
	float visibility = 1.0;
	
	if(depth < shadowCoord.z - 0.001) {
		visibility = 0.0;
	}
	float x = shadowCoord.x;
	float y = shadowCoord.y;
	if(x < 0 || x > 1 || y < 0 || y > 1) {
		visibility = 1.0;
	}

	vec3 sun = vec3(0, 1, 1);
	sun = normalize(sun);
	vec3 n = normalize(vNormal);
	float result = dot(sun, n);
	result = clamp(result, 0, 1);
	//vec3 color = texture(texSampler, vec2(vTex.x, 1 - vTex.y)).rgb;
	vec3 color = vec3(1);

	gl_FragColor = vec4(color * result * visibility * 0.7 + color * 0.3, 1);
	gl_FragColor = vec4(vNormal, 1.0);
}