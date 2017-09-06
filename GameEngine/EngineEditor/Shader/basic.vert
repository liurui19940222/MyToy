#version 400
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec4 VertexColor;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec4 color;

void main()
{
	color = VertexColor;
	gl_Position = P * V * M * vec4(VertexPosition, 1.0);
}