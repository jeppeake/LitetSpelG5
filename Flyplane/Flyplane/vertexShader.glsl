#version 420
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex;
uniform mat4 modelMatrix;
uniform mat4 ViewProjMatrix;

uniform mat4 shadowMatrix;
uniform mat4 terrainShadowMatrix;

out vec3 Pos;
out vec3 Normal;
out vec2 Tex;
out vec3 ShadowSpace;
out vec3 TerrainShadowSpace;


void main() {
	vec4 modelPos = modelMatrix * vec4(pos, 1.0);

	Pos = modelPos.xyz;
	Tex = tex;
	Normal = transpose(inverse(mat3(modelMatrix)))*normal;

	ShadowSpace = (shadowMatrix * modelPos).xyz;
	TerrainShadowSpace = (terrainShadowMatrix * modelPos).xyz;

	gl_Position = ViewProjMatrix * modelPos;
}