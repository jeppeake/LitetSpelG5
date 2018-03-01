#version 420
uniform sampler2D texSampler;

uniform sampler2D shadowMap;
uniform sampler2D terrainShadowMap;

uniform mat4 shadowMatrix;
uniform vec3 cameraPos;

uniform int drawShadows;

uniform vec3 sunDir;

in vec3 Pos;
in vec3 Normal;
in vec2 Tex;
in vec3 ShadowSpace;
in vec3 TerrainShadowSpace;


float testShadow(vec3 shadowCoord, sampler2D sampler, float c) {
//float testShadow(vec3 shadowCoord, sampler2D sampler, ivec2 offset, float c) {
	//float depth = textureOffset(sampler, shadowCoord.xy, offset).r;
	float depth = texture(sampler, shadowCoord.xy).r;
	float visibility = 1.0;
	
	vec3 normal = normalize(Normal);
	vec3 sun = sunDir;
	float cosa = clamp(dot(normal, sun), 0.0, 1.0);
	float bias = c*tan(acos(cosa));
	//bias = clamp(bias, 0.0, 0.01);

	if(depth < shadowCoord.z-bias) {
		visibility = 0.0;
	}
	float x = shadowCoord.x;
	float y = shadowCoord.y;
	float z = shadowCoord.z;
	if(x < 0 || x > 1 || y < 0 || y > 1 || z > 1) {
		visibility = 1.0;
	}
	return visibility;
}

float shadow(vec3 shadowCoord, sampler2D sampler, float c) {
	//float result = 0;

	/*
	const int hsize = 2;
	for(int i = -hsize; i <= hsize; i++) {
		for(int j = -hsize; j <= hsize; j++) {
			result += testShadow(shadowCoord, sampler, ivec2(i,j), c);
		}
	}
	*/
	//float samples = pow(float(hsize)*2.0+1.0, 2);

	return testShadow(shadowCoord, sampler, c);

	//return result; // / samples;
}

void main() {
	float visibility = 1.0;

	if(drawShadows!=0) {
		float visibility1 = shadow(ShadowSpace, shadowMap, 0.0002);
		float visibility2 = shadow(TerrainShadowSpace, terrainShadowMap, 0.0);
		visibility = min(visibility1, visibility2);
	}

	vec3 color = texture(texSampler, vec2(Tex.x, 1 - Tex.y)).rgb;


	vec3 sun = normalize(vec3(0, 1, 1));
	vec3 n = normalize(Normal);

	float diffuse = dot(sun, n);
	diffuse = clamp(diffuse, 0, 1);
	
	vec3 look = normalize(cameraPos - Pos);
	vec3 h = normalize(look + sun);
	float specular = pow(clamp(dot(n, h), 0.0, 1.0), 100.0);

	vec3 lighting;
	lighting += color * diffuse * visibility * 0.7;
	lighting += color * specular * visibility;
	lighting += color * 0.3;
	gl_FragColor = vec4(lighting, 1);
	
}