#version 420
layout(location = 0) in vec2 uv;
uniform mat4 modelMatrix;
uniform mat4 ViewProjMatrix;

uniform sampler2D heightmap;

uniform vec2 offset;
const vec2 size = vec2(1024/4);
const vec2 heightmapSize = vec2(1024);
const vec3 scale = 1.0*vec3(1, 100, 1);

out vec3 Pos;
out vec3 Normal;
//out vec2 Tex;




float noise(vec2 n);

float sampleHeightmap(vec2 uv, ivec2 offset) {
	float result = 0;
	result += textureOffset(heightmap, uv, offset).r;
	//result += noise(uv);
	return result;
}

float sampleHeightmap(vec2 uv) {
	return sampleHeightmap(uv, ivec2(0));
}

void main() {
	vec2 pos2d = uv*size + offset;
	vec2 hmUV = pos2d/heightmapSize;

	float height = sampleHeightmap(hmUV); 

	vec3 pos = scale*vec3(pos2d.x, height, pos2d.y);

	Pos = (modelMatrix * vec4(pos, 1.0)).xyz;
	
	vec4 h;
	h[0] = sampleHeightmap(hmUV, ivec2(0, -1));
	h[1] = sampleHeightmap(hmUV, ivec2(-1, 0));
	h[2] = sampleHeightmap(hmUV, ivec2(1, 0));
	h[3] = sampleHeightmap(hmUV, ivec2(0, 1));

	Normal.z = h[0] - h[3];
	Normal.x = h[1] - h[2];
	Normal.y = 1.0/255.0;
	Normal = normalize(Normal);
	//Normal = vec3(height);
	//Normal = vec3(modelMatrix * vec4(normal, 0));



	gl_Position = ViewProjMatrix * modelMatrix * vec4(pos, 1.0);
	//gl_Position = vec4(uv, 0.0, 1.0);
}


float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float noise(vec2 n) {
	const vec2 d = vec2(0.0, 1.0);
  vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
	return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}