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
const float life = 8.f;
uniform vec3 spawn;
uniform vec3 velocity;
layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

float rand(float n);

const vec3 smoke = vec3(0.7);
const vec3 light = vec3(1.0, 0.9, 0.6);

void main()
{


	uint gid = gl_GlobalInvocationID.x;
	if(Positions[gid].xyz == vec3(0))
	{
		Positions[gid].xyz = spawn;
		Lives[gid] = life * (rand(float(gid) + 1000) + 1)/2;
		Colors[gid].rgb = light;

		vec3 vel;
		vel.x = rand(float(gid + dt*10));
		vel.y = rand(float(gid + 1000 + dt*10));
		vel.z = rand(float(gid + 2000 + dt*10));
		Velocities[gid].xyz = 1 * normalize(vel);
	}
	else
	{
		if(Lives[gid] < life*0.01) {
			
			Colors[gid].rgb = light;
			vec3 vel;
			vel.x = rand(float(gid + dt*10));
			vel.y = rand(float(gid + 1000 + dt*10));
			vel.z = rand(float(gid + 2000 + dt*10));
			Velocities[gid].xyz = 1 * normalize(vel);

			Positions[gid].xyz = spawn;
			Colors[gid].a = 1;
		} else {
			vec3 dir;
			dir.x = rand(gid*23);
			dir.z = rand(gid*27);
			dir = normalize(dir);
			dir += vec3(0, 13, 0);
			Velocities[gid].xyz = mix(Velocities[gid].xyz, dir, 1-pow(0.1, dt));
			
			Colors[gid].rgb = smoke;

			Colors[gid].a = pow(1-Lives[gid] / life, 2);
		}
		//Positions[gid].xyz = spawn + direction * Lives[gid];
	}
	Lives[gid] += dt;
	if(Lives[gid] >= life)
	{
		Lives[gid] = 0;
	}
}

// [-1, 1]
float rand(float n) { 
	return 2.0*fract(sin(n * 12.9898) * 43758.5453)-1.0;
}