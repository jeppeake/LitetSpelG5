#version 420
uniform sampler2D texSampler;
uniform sampler2D shadowMap;
uniform mat4 shadowMatrix;
varying vec3 Pos;
varying vec3 Normal;
varying vec2 Tex;


void main() {
	vec3 shadowCoord = (shadowMatrix * vec4(Pos, 1)).xyz;
	float depth = texture(shadowMap, shadowCoord.xy).r;
	float visibility = 1.0;
	if(depth < shadowCoord.z - 0.00001) {
		visibility = 0.0;
	}

	vec3 sun = vec3(0, 1, 1);
	sun = normalize(sun);
	vec3 n = normalize(Normal);
	float result = dot(sun, n);
	result = clamp(result, 0, 1);
	vec3 color = texture(texSampler, vec2(Tex.x, 1 - Tex.y)).rgb;
	gl_FragColor = vec4(color * result, 1) * visibility + vec4(color * 0.2, 1);
	//gl_FragColor = vec4(vec3(shadowCoord.xy,0), 1);
}