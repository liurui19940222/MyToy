#version 400
layout(location = 0) in vec4 a_VertexPosition;
layout(location = 1) in vec2 a_Texcoord;
layout(location = 2) in vec4 a_TexRange;
layout(location = 3) in mat4 a_ModelMatrix;

uniform mat4 u_V;
uniform mat4 u_P;
uniform float u_Time;

out VERTEX{
	vec2 uv;
	vec4 range;
}vertex;

void main()
{
	vertex.uv = a_Texcoord;
	vertex.range = a_TexRange;
	//vertex.range = vec4(0.25 * (floor(u_Time) + gl_InstanceID), 0, 0.25, 1);
	gl_Position = u_P * u_V * a_ModelMatrix * a_VertexPosition;
}