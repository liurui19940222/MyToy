#version 400
layout(location = 0) in vec3 VertexPosition;
layout(location = 2) in vec2 VertexCoord;

uniform mat4 MVP;

out vec2 uv;

void main()
{
	uv = VertexCoord;
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}