#version 420
uniform sampler2D texSampler;

uniform sampler2D shadowMap;
uniform sampler2D terrainShadowMap;

uniform mat4 shadowMatrix;
uniform vec3 cameraPos;

uniform int drawShadows;

uniform vec3 sunDir;

out vec4 outColor;


in vec3 Pos;
in vec3 Normal;
in vec2 Tex;

in vec3 ShadowNormal;
in vec3 ShadowSpace;

in vec3 TerrainShadowNormal;
in vec3 TerrainShadowSpace;

float rand(float n) { 
	return 2.0*fract(sin(n * 12.9898) * 43758.5453)-1.0;
}

//float testShadow(vec3 shadowCoord, sampler2D sampler, float c) {
float testShadow(vec3 shadowCoord, sampler2D sampler, vec2 offset, float c) {
	float depth = texture(sampler, shadowCoord.xy + offset).r;
	float visibility = 1.0;

	float currFragDepth = shadowCoord.z;

	if(depth < currFragDepth) {
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

float shadow(vec3 shadowCoord, vec3 shadowNormal, sampler2D sampler, float c) {
	float result = 0;
	/*
	//vec3 xDir = cross(shadowNormal, vec3(0,1,0));
	//vec3 yDir = cross(shadowNormal, vec3(-1,0,0));

	const int hsize = 5;
	for(int i = -hsize; i <= hsize; i++) {
		for(int j = -hsize; j <= hsize; j++) {
			
		}
	}
	float samples = pow(float(hsize)*2.0+1.0, 2);
	*/

	float samples = 20;

	float div = 0;

	for(float i = 0; i < samples; i++) {
		vec2 offset;
		offset.x = rand(i + shadowCoord.x);
		offset.y = rand(-i-1 - shadowCoord.y);
		offset = rand(i * i + shadowCoord.z) * normalize(offset);
		float len = length(offset);

		offset *= 0.001;

		float falloff = pow(smoothstep(1, 0, len), 0.8);

		vec3 coord = shadowCoord;
		result += testShadow(shadowCoord, sampler, offset, c) * falloff;
		div += falloff;
	}

	return result / div ;
}



void main() {
	float visibility = 1.0;

	if(drawShadows!=0) {
		float visibility1 = shadow(ShadowSpace, normalize(ShadowNormal), shadowMap, 0.0002);
		float visibility2 = shadow(TerrainShadowSpace, normalize(TerrainShadowNormal), terrainShadowMap, 0.0);
		visibility = min(visibility1, visibility2);
	}

	vec4 colora = texture(texSampler, vec2(Tex.x, 1 - Tex.y));

	vec3 color = colora.rgb;
	float alpha = colora.a;
	if(alpha < 1.0/255.0) {
		discard;
	}

	vec3 sun = normalize(vec3(0, 1, 1));
	vec3 n = normalize(Normal);

	float diffuse = dot(sun, n);
	diffuse = clamp(diffuse, 0, 1);
	
	vec3 look = normalize(cameraPos - Pos);
	vec3 h = normalize(look + sun);
	float specular = pow(clamp(dot(n, h), 0.0, 1.0), 100.0);

	vec3 lighting;
	lighting += color * diffuse * visibility * 0.8;
	lighting += color * specular * visibility;
	lighting += color * 0.2;

	float dist = length(cameraPos - Pos);
	float fog = pow(clamp(dist/30000.0, 0.0, 1.0), 2);
	vec3 fogColor = 0.9*vec3(100.0/255,149.0/255,234.0/255);

	float thickFog = pow(clamp(dist/30000.0, 0.0, 1.0), 2);
	vec3 thickFogColor = vec3(100.0/255,149.0/255,234.0/255);

	color = mix(lighting, vec3(fogColor), fog);
	color = mix(color, vec3(thickFogColor), thickFog);

	outColor = vec4(color, alpha);
	
	//gl_FragColor = vec4(ShadowNormal, 1);
	
}


