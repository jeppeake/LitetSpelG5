#version 420
layout(location = 0) in vec2 uv;
uniform mat4 modelMatrix;
uniform mat4 ViewProjMatrix;

uniform sampler2D heightmap;
uniform sampler2D materialmap;
uniform vec2 heightmapSize;
uniform vec3 heightmapPos;

uniform vec2 offset;
uniform vec2 patch_size;
uniform vec3 scale;

out vec3 Pos;
out vec3 Normal;
out vec2 Tex;
out vec3 Materials;

float rand(vec2 p);

float noise(vec2 n);

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
	n.z = h[0] - h[3];
	n.x = h[1] - h[2];
	n.y = 1.0/255.0;
	return normalize(n);
}


void main() {
	vec2 pos2d = uv*patch_size + offset;
	vec2 hmUV = pos2d/heightmapSize;

	float height = sampleHeightmap(hmUV); 
	//height = 0;
	//height = -patch_size.x/1400.0;

	vec3 pos = scale*vec3(pos2d.x, 255.0*height, pos2d.y) + heightmapPos;

	Normal = sampleNormal(hmUV);
	


	/*
	float pi = 3.1415;
	float h = pi*log(patch_size.x/heightmapSize.x);
	Normal.x = 0.5*sin(h)+0.5;
	Normal.y = 0.5*sin(h + 2*pi/3)+0.5;
	Normal.z = 0.5*sin(h + 4*pi/3)+0.5;
	
	//Normal = vec3(modelMatrix * vec4(normal, 0));
	vec3 n;
	n.x = fract(rand(pos.xz*100));
	n.y = fract(rand(pos.xz*100 + vec2(1000, 1000)));
	n.z = fract(rand(pos.xz*100 + vec2(2000, 2000)));
	Normal = mix(Normal, n, 0.35);
	*/

	Materials = texture(materialmap, hmUV).rgb;
	Tex = pos2d;
	Tex = hmUV;
	Pos = pos;
	gl_Position = ViewProjMatrix * vec4(pos, 1.0);
}


float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}
float noise(vec2 n) {
	const vec2 d = vec2(0.0, 1.0);
	vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
	return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}