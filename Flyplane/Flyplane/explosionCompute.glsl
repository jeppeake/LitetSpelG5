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
layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;
float rand(float n) { 
	return 2.0*fract(sin(n * 12.9898) * 43758.5453)-1.0;
}
void main()
{
	uint gid = gl_GlobalInvocationID.x;
	if(Positions[gid].xyz == vec3(0))
	{
		Positions[gid].xyz = spawn;
		Lives[gid] = life * (rand(float(gid) + 1000) + 1)/2;
		Colors[gid].xyz = vec3(1.0,0.0,0.0);
	}
	else
	{
		Velocities[gid].xyz -= Velocities[gid].xyz * dt;
		Positions[gid].xyz += Velocities[gid].xyz * dt;

		Colors[gid].g += Lives[gid] / 3.0 * dt;
		Colors[gid].a -= dt / 3.0;
	}
	Lives[gid] += dt;
	if(Lives[gid] >= life + 0.1*life*rand(float(gid)))
	{
		Lives[gid] -= life;
		Positions[gid].xyz = spawn + vec3(rand(gid),rand(-gid),rand(gid * 2 - 1)) *abs(300 * dt  * (rand(float(gid) + dt) + 1) + 2.5);

		vec3 vel = noise3(gid);
		Velocities[gid].xyz = 50 * vec3(rand(gid),rand(-gid),rand(gid * 2 - 1)) + 3 * normalize(vel);
	}
}