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
uniform vec3 velocity;
uniform vec3 up;

uniform int respawnID1;
uniform int respawnID2;
uniform int respawnID3;

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

float rand(float n);

void main()
{
	

	uint gid = gl_GlobalInvocationID.x;
	if(Positions[gid].xyz == vec3(0))
	{
		// INITIAL SPAWN

		Positions[gid].xyz = spawn;
		Lives[gid] = 0;
		Colors[gid].rgba = vec4(1);
	}
	else
	{
		// UPDATE
		if(gid == respawnID1 || gid == respawnID2 || gid == respawnID3) {
			Positions[gid].xyz = spawn;


			Colors[gid].a = smoothstep(5, 50, abs(dot(velocity, up)));
			//Colors[gid].a = 1;

			Lives[gid] = 0;
		}

		Colors[gid].a = mix(Colors[gid].a, 0, smoothstep(0.0, 2.0, Lives[gid]));
	}
	Lives[gid] += dt;
	
	/*
	
	if(Lives[gid] >= life)
	{
		// RESPAWN
		Lives[gid] -= life;
		Positions[gid].xyz = spawn - 1.2 * dt  * velocity * (rand(gid*3) + 1)/2;
	}
	*/
}

// [-1, 1]
float rand(float n) { 
	return 2.0*fract(sin(n * 12.9898) * 43758.5453)-1.0;
}