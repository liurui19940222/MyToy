#version 410
layout(location = 0) in vec4 a_VertexPosition;
layout(location = 1) in vec2 a_Texcoord;
layout(location = 2) in mat4 a_ModelMatrix;

uniform mat4 u_V;
uniform mat4 u_P;

out VERTEX{
	vec2 uv;
}vertex;

void main()
{
	vertex.uv = a_Texcoord;
	gl_Position = u_P * u_V * a_ModelMatrix * a_VertexPosition;
}