#version 400
layout(location = 0) in vec3 VertexPosition;
layout(location = 2) in vec2 VertexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 uv;

void main()
{
	uv = VertexCoord;
	gl_Position = P * V * M * vec4(VertexPosition, 1.0);
}