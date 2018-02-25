#version 420
layout(location = 0) in vec2 uv;

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

float rand(vec2 p);
float noise(vec2 n);
float noise(vec3 p);

float sampleHeightmap(vec2 uv, vec2 offset) {
	float result = 0;
	uv += offset/heightmapSize;
	result += texture(heightmap, uv).r;
	return result;
}

float sampleHeightmap(vec2 uv) {
	return sampleHeightmap(uv, vec2(0));
}



float calcWaterHeight(vec3 pos) {
	float multi = 150.0;
	float h1 = -300.0;
	float h2 = 1000.0;

	float result = waterHeight;

	float val = smoothstep(h1, h2, waterHeight - pos.y);
	val = pow(val, 1.5);

	
	float p = 2;
	float sumNoise = 0.0;
	for(int i = 0; i < 7; i++) {
		float fi = float(i)+1;
		vec2 plane = pow(fi, 1.5)*0.005 * vec2(pos.x, pos.z) + time*0.2*(1+fi);
		float ns = noise(vec3(plane, time*0.2));
		ns = pow(ns, p);
		ns /= pow(fi, 1.6);
		sumNoise += ns;
	}

	result += val*multi*sumNoise;

	return result;
}


void main() {
	vec2 pos2d = uv*patch_size + offset;
	vec2 hmUV = pos2d/heightmapSize;

	float height = sampleHeightmap(hmUV);

	vec3 pos = scale*vec3(pos2d.x, height, pos2d.y) + heightmapPos;

	float currWaterHeight = calcWaterHeight(pos);
	if(pos.y <= currWaterHeight) {
		pos.y = currWaterHeight;
	}

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