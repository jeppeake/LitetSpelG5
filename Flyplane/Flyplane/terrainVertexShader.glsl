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

uniform float waterHeight;

uniform float time;

out vec3 Pos;
out vec3 GeometryPos;
out vec3 Normal;
out vec2 Tex;
out vec3 Materials;
out vec3 Color;

float rand(vec2 p);

float noise(vec2 n);
float noise(vec3 p);

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

	float height = sampleHeightmap(hmUV);// 

	vec3 pos = scale*vec3(pos2d.x, height, pos2d.y) + heightmapPos;

	Normal = sampleNormal(hmUV);
	

	/*
	float pi = 3.1415;
	float h = pi*log(patch_size.x/heightmapSize.x);
	Normal.x = 0.5*sin(h)+0.5;
	Normal.y = 0.5*sin(h + 2*pi/3)+0.5;
	Normal.z = 0.5*sin(h + 4*pi/3)+0.5;
	vec3 n;
	n.x = fract(rand(pos.xz*100));
	n.y = fract(rand(pos.xz*100 + vec2(1000, 1000)));
	n.z = fract(rand(pos.xz*100 + vec2(2000, 2000)));
	Normal = mix(Normal, n, 0.35);
	*/


	Color = 1.1*vec3(0.376, 0.702, 0.22);
	float snowHeight = 3000 + 1100*(noise(pos.xz*0.001));
	if(pos.y > snowHeight) {
		Color = vec3(1);
	}
	if(dot(Normal, vec3(0,1,0)) < 0.38) {
		Color = vec3(0.4);
	}

	Materials = texture(materialmap, hmUV).rgb;
	Tex = pos2d;
	Tex = hmUV;
	Pos = pos;
	if(pos.y <= waterHeight) {

		float val = (waterHeight - pos.y)/1500;
		val = pow(clamp(val, 0.0, 1.0), 0.5);
		

		pos.y = waterHeight;
		Pos = pos;

		float p = 2;
		float sumNoise;
		sumNoise += pow(noise(vec3(pos.xz*0.005 + time*0.2, time*0.2)), p);
		sumNoise += 0.3*pow(noise(vec3(pos.xz*0.01 + time*0.3, time*0.3)), p);
		sumNoise += 0.08*pow(noise(vec3(pos.xz*0.02 + time*0.4, time*0.3)), p);
		sumNoise += 0.01*pow(noise(vec3(pos.xz*0.5 + time*0.4, time*0.3)), p);
		pos.y += val*150*sumNoise;
	}
	GeometryPos = pos;
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

float mod289(float x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 mod289(vec4 x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 perm(vec4 x){return mod289(((x * 34.0) + 1.0) * x);}

float noise(vec3 p){
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + a.zzzz;
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y) - 1.0;
}