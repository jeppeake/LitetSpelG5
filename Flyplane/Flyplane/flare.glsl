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
const float life = 3.f;

uniform float time;
uniform float dt;
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
		Lives[gid] = life * (rand(gid) + 1)/2;
	}
	else
	{
		if(gid < 20) {
			Colors[gid].rgb = light;
			Colors[gid].a = 0.6 + 0.4 * rand(gid + dt * 1000);
			
			Colors[gid].a *= smoothstep(3.5, 3.0, time);

			vec3 offset;
			offset.x = rand(20 * gid + dt * 100 + 100);
			offset.y = rand(30 * gid + dt * 150 + 200);
			offset.z = rand(40 * gid + dt * 200 + 300);
			offset = normalize(offset);

			Positions[gid].xyz = spawn + 0.05*rand(gid * 10 + dt * 200) * offset;
		} else {
			Velocities[gid].xyz -= 0.3*Velocities[gid].xyz * dt;
			Positions[gid].xyz += Velocities[gid].xyz * dt;
			Positions[gid].xyz += 0.5 * velocity * dt;
			Colors[gid].a = pow(smoothstep(life, 0, Lives[gid]), 4)*0.5;
		}
		Colors[gid].a *= smoothstep(5, 3, time);
	}
	Lives[gid] += dt;
	if(Lives[gid] >= life)
	{
		Lives[gid] -= life;

		vec3 offset;
		offset.x = rand(20 * gid + dt * 100 + 100);
		offset.y = rand(30 * gid + dt * 150 + 200);
		offset.z = rand(40 * gid + dt * 200 + 300);
		offset = normalize(offset);

		Positions[gid].xyz = spawn - abs(rand(gid*10 + dt*500)) * velocity * dt;
		Positions[gid].xyz += 0.05*rand(gid * 10 + dt * 200) * offset;



		vec3 vel;
		vel.x = rand(-20 * gid - dt * 100 - 100);
		vel.y = rand(-30 * gid - dt * 150 - 200);
		vel.z = rand(-40 * gid - dt * 200 - 300);
		vel = normalize(vel);

		Velocities[gid].xyz = 0.05*rand(gid * 20 + dt * 200) * vel;
	}
}

// [-1, 1]
float rand(float n) { 
	return 2.0*fract(sin(n * 12.9898) * 43758.5453)-1.0;
}