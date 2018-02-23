#version 430
layout(location = 0) in vec4 a_VertexPosition;
layout(location = 1) in vec2 a_Texcoord;
layout(location = 2) in vec4 a_TexRange;
layout(location = 3) in vec4 a_Color;
layout(location = 4) in vec4 a_Rect;
layout(location = 5) in mat4 a_ModelMatrix;

uniform mat4 u_M;
uniform mat4 u_V;
uniform mat4 u_P;

out VERTEX{
	vec2 uv;
	vec4 range;
	vec4 color;
}vertex;

void main()
{
	vec4 pos = a_VertexPosition;
	pos.xy *= a_Rect.zw * 2;
	vertex.uv = a_Texcoord;
	vertex.range = a_TexRange;
	vertex.color = a_Color;
	gl_Position = u_P * u_V * u_M * a_ModelMatrix * pos;
}