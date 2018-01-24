#version 400
layout(location = 0) in vec3 a_VertexPosition;
layout(location = 2) in vec2 a_VertexCoord;

uniform vec2 u_InstanceSize;
uniform mat4 u_M;
uniform mat4 u_V;
uniform mat4 u_P;

out vec2 uv;

void main()
{
	vec3 position = a_VertexPosition;
	position.xy *= u_InstanceSize;
	uv = a_VertexCoord;
	gl_Position = u_P * u_V * u_M * vec4(a_VertexPosition, 1.0);
}