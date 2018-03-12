#version 420
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex;
uniform mat4 modelMatrix;
uniform mat4 ViewProjMatrix;

uniform mat4 shadowMatrix;
uniform mat4 terrainShadowMatrix;

uniform vec3 sunDir;

out vec3 Pos;
out vec3 Normal;
out vec2 Tex;

out vec3 ShadowNormal;
out vec3 ShadowSpace;

out vec3 TerrainShadowNormal;
out vec3 TerrainShadowSpace;


void main() {
	vec4 modelPos = modelMatrix * vec4(pos, 1.0);

	Pos = modelPos.xyz;
	Tex = tex;
	Normal = transpose(inverse(mat3(modelMatrix)))*normal;

	ShadowNormal = normalize((shadowMatrix * vec4(Normal, 0)).xyz);
	ShadowSpace = (shadowMatrix * vec4(modelPos.xyz + 0.03*Normal, 1)).xyz;

	TerrainShadowNormal = normalize((terrainShadowMatrix * vec4(Normal, 0)).xyz);
	TerrainShadowSpace = (terrainShadowMatrix * vec4(modelPos.xyz + 1.0*Normal, 1)).xyz;

	gl_Position = ViewProjMatrix * modelPos;
}