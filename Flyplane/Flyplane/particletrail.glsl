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
uniform vec3 direction;
layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

float rand(float n);

void main()
{
	uint gid = gl_GlobalInvocationID.x;
	if(Positions[gid].xyz == vec3(0))
	{
		Positions[gid].xyz = spawn;
		Lives[gid] = life * (rand(float(gid) + 1000) + 1)/2;
	}
	else
	{
		Velocities[gid].xyz -= 0.3*Velocities[gid].xyz * dt;
		Positions[gid].xyz += Velocities[gid].xyz * dt;

		Colors[gid].a = pow(1-Lives[gid] / life, 4);

		//Positions[gid].xyz = spawn + direction * Lives[gid];
	}
	Lives[gid] += dt;
	if(Lives[gid] >= life + 0.1*life*rand(float(gid)))
	{
		Lives[gid] -= life;
		Positions[gid].xyz = spawn + direction*abs(300 * dt  * (rand(float(gid) + dt) + 1) + 10.5);

		vec3 vel;
		vel.x = rand(float(gid + dt*10));
		vel.y = rand(float(gid + 1000 + dt*10));
		vel.z = rand(float(gid + 2000 + dt*10));
		Velocities[gid].xyz = 20 * direction + 3 * normalize(vel);
	}
}

// [-1, 1]
float rand(float n) { 
	return 2.0*fract(sin(n * 12.9898) * 43758.5453)-1.0;
}