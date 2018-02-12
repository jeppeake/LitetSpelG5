#version 420
layout(points) in;
layout(line_strip, max_vertices = 12) out;

uniform vec3 cameraPos;
uniform mat4 aspectMatrix;
uniform float scale;

in vec3 Pos[];

void main() {
	vec3 dir = cameraPos - Pos[0];
	vec3 up = vec3(0, 1, 0);
	dir = normalize(dir);
	vec3 right = cross(dir, up);
	right = normalize(right);
	up = cross(dir, right);
	up = normalize(up);
	right = right * scale;
	vec3 right2 = right * 0.3;
	up = up * scale;
	vec3 up2 = up * 0.3;

	//höger upp
	gl_Position = aspectMatrix * vec4((Pos[0] + right2 + up), 1);//right * scale * 0.3 + up * scale
	EmitVertex();				 										
								 										
	gl_Position = aspectMatrix * vec4((Pos[0] + right + up), 1);//right * scale + up * scale
	EmitVertex();
	
	gl_Position = aspectMatrix * vec4((Pos[0] + right + up2), 1);//right * scale + up * scale * 0.3
	EmitVertex();
	EndPrimitive();				 										
				  		
	//höger ner
	gl_Position = aspectMatrix * vec4((Pos[0] + right - up2), 1);//right * scale - up * scale * 0.3
	EmitVertex(); 														
				  														
	gl_Position = aspectMatrix * vec4((Pos[0] + right - up), 1);//right * scale - up * scale
	EmitVertex(); 			
	
	gl_Position = aspectMatrix * vec4((Pos[0] + right2 - up), 1);//right * scale * 0.3 - up * scale
	EmitVertex(); 
	EndPrimitive();				 										
	
	//vänster ner
	gl_Position = aspectMatrix * vec4((Pos[0] - right2 - up), 1);//right * scale * 0.3 - up * scale
	EmitVertex(); 				 										
				  				 										
	gl_Position = aspectMatrix * vec4((Pos[0] - right - up), 1);//right * scale - up * scale
	EmitVertex(); 
	
	gl_Position = aspectMatrix * vec4((Pos[0] - right - up2), 1);// right * scale - up * scale * 0.3
	EmitVertex(); 
	EndPrimitive();				 										

	//vänster upp
	gl_Position = aspectMatrix * vec4((Pos[0] - right + up2), 1);//right * scale + up * scale * 0.3
	EmitVertex(); 				 										

	gl_Position = aspectMatrix * vec4((Pos[0] - right + up), 1);//right * scale + up * scale
	EmitVertex();

	gl_Position = aspectMatrix * vec4((Pos[0] - right2 + up), 1);//right * scale * 0.3 + up * scale
	EmitVertex();
	EndPrimitive();
}