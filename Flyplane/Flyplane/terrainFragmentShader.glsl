#version 420

uniform sampler2D shadowMap;
uniform sampler2D terrainShadowMap;

out vec4 outColor;

in vec3 vPos;
in vec3 vNormal;
in vec2 vTex;
in vec3 vMaterials;
in vec3 vGeometryPos;
in vec3 vShadowSpace;
in vec3 vTerrainShadowSpace;
flat in vec3 vColor;


uniform vec3 sunDir;
uniform sampler2D heightmap;
uniform sampler2D materialmap;
uniform sampler2D material1;
uniform sampler2D material2;
uniform sampler2D material3;
uniform vec2 heightmapSize;

// in world
uniform float waterHeight;

uniform vec3 cameraPos;

uniform vec3 scale;

float noise(vec2 pos);

float detail(vec2 pos) {
	return 0.1*(noise(pos*0.1) + 0.5*noise(pos*0.5));
}

float sampleHeightmap(vec2 uv, vec2 offset) {
	float result = 0;
	uv += offset/heightmapSize;
	result += texture(heightmap, uv).r;
	//result += 0.1*(noise(uv*3) + 0.5*noise(uv*10.0));
	return result;
}
float sampleHeightmap(vec2 uv) {
	return sampleHeightmap(uv, vec2(0));
}

vec3 sampleNormal(vec2 hmUV) {
	vec4 h;
	h[0] = sampleHeightmap(hmUV, vec2(0, -1));
	h[1] = sampleHeightmap(hmUV, vec2(-1, 0));
	h[2] = sampleHeightmap(hmUV, vec2(1, 0));
	h[3] = sampleHeightmap(hmUV, vec2(0, 1));

	float ratioX = scale.x/scale.y;
	float ratioZ = scale.z/scale.y;
	vec3 n;
	n.z = ratioZ * (h[0] - h[3]);
	n.x = ratioX * (h[1] - h[2]);
	n.y = 2 * ratioX*ratioZ;
	return normalize(n);
}

struct Material {
	vec3 color;
	float specular;
};

vec3 rgb(float r, float g, float b) {
	return vec3(r, g, b) / 255.0;
}


const vec3 colorSnow = vec3(1);
const vec3 colorGrass = rgb(151, 169, 79);
const vec3 colorDarkGrass = 0.6*rgb(151, 169, 79);
const vec3 colorSand = rgb(194, 178, 128);
const vec3 colorRock = rgb(81,79,86);
const vec3 colorWater = rgb(30, 90, 190);


Material chooseMat(vec3 pos, vec3 normal, float biome, float geomHeight) {
	Material result;
	result.color = colorGrass;
	result.specular = 0;
	float leaning = dot(normal, vec3(0,1,0));

	float snowHeight = scale.y*0.8 - scale.y*0.2*(noise(pos.xz*0.001));

	// leaning grass
	if(leaning < 0.9) {
		result.color = colorDarkGrass;
	}

	// snow
	if(pos.y > snowHeight) {
		result.color = colorSnow;
	}

	// sand
	if(pos.y <= waterHeight+50 + 50*noise(pos.xz*0.0001) && leaning > 0.6) {
		result.color = colorSand;
	}

	result.color = mix(result.color, colorSand, biome);
	
	// rock wall
	if(leaning < 0.5) {
		result.color = colorRock;
	}

	

	// water
	if(pos.y <= waterHeight+2) {
		float val = 0;
		val = smoothstep(1000.0, -10.0, waterHeight - pos.y);
		val = pow(val, 60);
		result.color = mix(colorWater, result.color, val);
		result.specular = 1;
		
	}

	return result;
}

float testShadow(vec3 shadowCoord, sampler2D sampler) {
//float testShadow(vec3 shadowCoord, sampler2D sampler, ivec2 offset) {
	//float depth = textureOffset(sampler, shadowCoord.xy, offset).r;
	float depth = texture(sampler, shadowCoord.xy).r;
	float visibility = 1.0;
	
	vec3 normal = vNormal;
	vec3 sun = sunDir;
	float cosa = clamp(dot(normal, sun), 0.0, 1.0);
	float bias = 0.005*tan(acos(cosa));
	bias = clamp(bias, 0.0, 0.01);

	if(depth < shadowCoord.z-bias) {
		visibility = 0.05;
	}
	float x = shadowCoord.x;
	float y = shadowCoord.y;
	float z = shadowCoord.z;
	if(x < 0 || x > 1 || y < 0 || y > 1 || z > 1) {
		visibility = 1.0;
	}


	return visibility;
}

float kernel[5*5] = {
	0.003765,	0.015019,	0.023792,	0.015019,	0.003765,
	0.015019,	0.059912,	0.094907,	0.059912,	0.015019,
	0.023792,	0.094907,	0.150342,	0.094907,	0.023792,
	0.015019,	0.059912,	0.094907,	0.059912,	0.015019,
	0.003765,	0.015019,	0.023792,	0.015019,	0.003765
};

float shadow(vec3 shadowCoord, sampler2D sampler) {
	/*
	float result = 0;

	const int hsize = 2;
	for(int i = -hsize; i <= hsize; i++) {
		for(int j = -hsize; j <= hsize; j++) {
			result += kernel[(i+hsize) + (j+hsize)*5]*testShadow(shadowCoord, sampler, ivec2(i,j));
		}
	}
	float samples = pow(float(hsize)*2.0+1.0, 2);
	
	return result;
	*/
	return testShadow(shadowCoord, sampler);
}

void main() {
	float visibility1 = shadow(vShadowSpace, shadowMap);
	float visibility2 = shadow(vTerrainShadowSpace, terrainShadowMap);
	float visibility = min(visibility1, visibility2);
	
	float x = vTerrainShadowSpace.x;
	float y = vTerrainShadowSpace.y;
	float padding = 0.1;
	float falloff = smoothstep(1, 1-padding, x);
	falloff *= smoothstep(0, padding, x);
	falloff *= smoothstep(1, 1-padding, y);
	falloff *= smoothstep(0, padding, y);
	visibility = mix(1, visibility, falloff);


	//visibility = visibility2;

	float biome = texture(materialmap, vec2(vTex.x, vTex.y)).r;
	vec3 matNormal = sampleNormal(vTex);


	Material mat = chooseMat(vPos, matNormal, biome, vGeometryPos.y);
	vec3 color = mat.color;
	

	vec3 sun = normalize(sunDir);
	vec3 n = normalize(vNormal);

	float diffuse = clamp(dot(sun, n), 0, 1);

	vec3 look = normalize(cameraPos - vPos);
	vec3 h = normalize(look + sun);
	float specular = pow(clamp(dot(n, h), 0.0, 1.0), 1000.0);

	vec3 lighting = vec3(0);
	lighting += color * diffuse * visibility * 0.8 * (1.3 - mat.specular);
	lighting += specular * visibility * mat.specular;
	lighting += color * 0.2;


	float dist = length(cameraPos - vPos);
	float fog = pow(clamp(dist/30000.0, 0.0, 1.0), 2);
	vec3 fogColor = 0.9*vec3(100.0/255,149.0/255,234.0/255);

	float thickFog = pow(clamp(dist/30000.0, 0.0, 1.0), 2);
	vec3 thickFogColor = vec3(100.0/255,149.0/255,234.0/255);

	color = mix(lighting, vec3(fogColor), fog);
	color = mix(color, vec3(thickFogColor), thickFog);

	outColor = vec4(color, 1);

	//outColor = vec4(vec3(falloff), 1);
	//outColor = vec4(vec3(vTerrainShadowSpace.z), 1);

	//float depth = texture(terrainShadowMap, vTerrainShadowSpace.xy).r;
	//outColor = vec4(vec3(depth), 1);
}



float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}
float noise(vec2 n) {
	const vec2 d = vec2(0.0, 1.0);
	vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
	return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}