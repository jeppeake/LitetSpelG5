#version 430 core
layout(std430, binding=7) buffer Pos
{
	vec4 Positions[];
};
layout(std430, binding=8) buffer Vel
{
	vec4 Velocities[];
};
layout(std430, binding=9) buffer Life
{
	float Lives[];
};
layout(std430, binding=10) buffer Col
{
	vec4 Colors[];
};
uniform float dt;
uniform float life;
uniform vec3 spawn;
uniform float explosionRadius;
layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;


float rand(float n) { 
	return 2.0*fract(sin(n * 12.9898) * 43758.5453)-1.0;
}
float noise(vec3 p);


const vec3 flash = vec3(1.0, 0.9, 0.1);
const vec3 fire = vec3(1.0, 0.1, 0.0);
const vec3 smoke = vec3(0.1);

const float every = 30;

void main()
{
	uint gid = gl_GlobalInvocationID.x;
	if(Positions[gid].xyz == vec3(0))
	{
		Positions[gid].xyz = spawn + 10 * normalize(vec3(rand(gid * 2), rand(gid * 3), rand(gid * 5)));
		Lives[gid] = 0;

		

		vec3 vel;
		vel.x = rand(gid * 17);
		vel.y = rand(gid * 7);
		vel.z = rand(gid * 11);
		vel = normalize(vel);

		if(mod(gid, every) < 1) {
			Colors[gid].xyz = smoke;
		} else {
			vec3 mixer;
			mixer.x = abs(noise(vec3(12*vel+ vec3(20))));
			mixer.y = abs(noise(vec3(12*vel+ vec3(-10))));
			mixer.z = abs(noise(vec3(12*vel + vec3(10))));
			mixer = normalize(pow(normalize(mixer), vec3(3)));
			Colors[gid].xyz = mixer.x * flash + mixer.y * fire + mixer.z * smoke;
		}

		
		Velocities[gid].xyz = 100.0 * (sqrt(explosionRadius) * abs(noise(vec3(12*vel)))) * pow(rand(gid * 13)*0.5+0.5, 0.3)* vel;
	}
	else
	{
		if(mod(gid, every) < 1) {
			Velocities[gid].xyz += vec3(0, -9.82, 0) * dt;
			Velocities[gid].xyz -= Velocities[gid].xyz * (1 - pow(0.8, dt));
		} else {
			Velocities[gid].xyz -= Velocities[gid].xyz * (1 - pow(0.01, dt));
		}
		Positions[gid].xyz += Velocities[gid].xyz * dt;

		Colors[gid].rgb = mix(Colors[gid].rgb, smoke, pow(smoothstep(0, 2*life, Lives[gid]), 1));
		Colors[gid].a = pow(smoothstep(life, 0, Lives[gid]), 1);
	}
	Lives[gid] += dt;
}


float mod289(float x){return x - floor(x / 289.0) * 289.0;}
vec4 mod289(vec4 x){return x - floor(x / 289.0) * 289.0;}
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

    vec4 o1 = fract(k3 / 41.0);
    vec4 o2 = fract(k4 / 41.0);

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y) - 1.0;
}