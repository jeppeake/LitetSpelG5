#version 420

uniform sampler2D shadowMap;
uniform mat4 shadowMatrix;
in vec3 vPos;
in vec3 vNormal;
in vec2 vTex;
in vec3 vMaterials;
flat in vec3 vColor;

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

	vec3 n;
	n.z = scale.z*(h[0] - h[3]);
	n.x = scale.x*(h[1] - h[2]);
	n.y = 1;// scale.y; //*1.0/65535.0
	return normalize(n);
}

struct Material {
	vec3 color;
	float specular;
};

vec3 rgb(float r, float g, float b) {
	return vec3(r, g, b) / 255.0;
}

Material chooseMat(vec3 pos, vec3 normal) {
	Material result;
	result.color = rgb(199,159,72);
	result.specular = 0;
	float leaning = dot(normal, vec3(0,1,0));

	float snowHeight = 21000 + 500*(noise(pos.xz*0.001));

	
	if(leaning < 0.6) {
		result.color = rgb(153,102,47);
	}

	// snow
	if(pos.y > snowHeight) {
		result.color = vec3(1);
	}

	// sand
	if(pos.y <= waterHeight+50 + 50*noise(pos.xz*0.0001) && leaning > 0.6) {
		result.color = vec3(194, 178, 128)/255.0;
	}
	
	// rock wall
	if(leaning < 0.35) {
		result.color = rgb(81,79,86);
	}

	// water
	if(pos.y <= waterHeight+5) {
		result.color = vec3(30, 90, 190)/255.0;
		result.specular = 1;
	}

	return result;
}

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

	//vec3 color = texture(materialmap, vec2(vTex.x, vTex.y)).rgb;
	vec3 matNormal = sampleNormal(vTex);


	Material mat = chooseMat(vPos, matNormal);
	vec3 color = mat.color;

	// for triangle colors
	//color = vColor;
	

	vec3 sun = vec3(0, 1, 1);
	sun = normalize(sun);
	vec3 n = normalize(vNormal);

	float diffuse = clamp(dot(sun, n), 0, 1);

	vec3 look = normalize(cameraPos - vPos);
	vec3 h = normalize(look + sun);
	float specular = pow(clamp(dot(n, h), 0.0, 1.0), 1000.0);


	vec3 lighting;
	lighting += color * diffuse * visibility * 0.7 * (1.3 - mat.specular);
	lighting += color * specular * visibility * mat.specular;
	lighting += color * 0.3;


	float dist = length(cameraPos - vPos);
	float fog = pow(clamp(dist/48000.0, 0.0, 1.0), 1.5);
	vec3 fogColor = 0.95*vec3(100.0/255,149.0/255,234.0/255);

	color = mix(lighting, vec3(fogColor), fog);
	gl_FragColor = vec4(color, 1);
	//gl_FragColor = vec4(matNormal, 1);
}



float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}
float noise(vec2 n) {
	const vec2 d = vec2(0.0, 1.0);
	vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
	return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}