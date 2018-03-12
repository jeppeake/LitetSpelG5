#version 430 core

layout(local_size_x = LOCAL_SIZE_X, local_size_y = 1, local_size_z = 1) in;

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

// [-1, 1]
float rand(float n) { 
	return 2.0*fract(sin(n * 12.9898) * 43758.5453)-1.0;
}

void main()
{
	uint gid = gl_GlobalInvocationID.x;
	if(Positions[gid].xyz == vec3(0))
	{
		Lives[gid] = abs(rand(gid + 1000)) * life;
		Positions[gid].xyz = spawn - direction * 100 + vec3(rand(gid), rand(gid * gid), rand(gid + 2000)) * 750;
		Colors[gid].rgba = vec4(0.7,0.7,0.5, rand(gid + 5000));
	}
	else 
	{
		
	}

	Lives[gid] += dt;
	if(Lives[gid] >= life)
	{
		Positions[gid].xyz = spawn - direction * 100 + vec3(rand(gid), rand(gid * gid), rand(gid + 2000)) * 750;
		Colors[gid].rgba = vec4(0.7,0.7,0.5, rand(gid + 5000));
		Lives[gid] -= life;
	}
}