#version 420
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
//layout(line_strip, max_vertices = 4) out;

uniform mat4 shadowMatrix;
uniform mat4 terrainShadowMatrix;

in vec3 Pos[];
in vec3 Normal[];
in vec2 Tex[];
in vec3 Materials[];
in vec3 Color[];
in vec3 GeometryPos[];

out vec3 vPos;
out vec3 vNormal;
out vec2 vTex;
out vec3 vMaterials;
out vec3 vGeometryPos;
out vec3 vShadowSpace;
out vec3 vTerrainShadowSpace;
flat out vec3 vColor;




void main() {
	vec3 v1 = GeometryPos[0] - GeometryPos[1];
	vec3 v2 = GeometryPos[0] - GeometryPos[2];
	vNormal = normalize(cross(v1, v2));
	for(int i = 0; i < 3; i++) {
		vPos = Pos[i];
		//vNormal = Normal[i];
		vGeometryPos = GeometryPos[i];

		vShadowSpace = (shadowMatrix * vec4(vGeometryPos - 1*vNormal, 1.0)).xyz;
		vTerrainShadowSpace = (terrainShadowMatrix * vec4(vGeometryPos + 1.0*vNormal, 1.0)).xyz;

		vTex = Tex[i];
		vColor = Color[i];
		vMaterials = Materials[i];
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	/*
	// for wireframe
	vPos = Pos[0];
	vNormal = Normal[0];
	vTex = Tex[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	*/
	EndPrimitive();
	
}

