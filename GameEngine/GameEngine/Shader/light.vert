#version 400

layout(location = 0) in vec3 VertexPosition;
layout(location = 2) in vec2 TextureCoord;
layout(location = 3) in vec3 VertexNormal;

const vec3 lightDir = vec3(0.3, -0.4, 0.3);
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 uv;
out vec3 normal;
out vec3 eyeLight;
out vec3 eyeDir;

void main()
{
	mat4 mv = V * M;
	eyeDir = normalize(-(mv * vec4(VertexPosition, 1.0)).xyz);
	eyeLight = normalize((V * vec4(lightDir, 0.0)).xyz);
	normal = normalize((mv * vec4(VertexNormal, 0.0)).xyz);
	uv = TextureCoord;
	gl_Position = P * V * M * vec4(VertexPosition, 1.0);
}