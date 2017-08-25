#version 400

layout(location = 0) in vec3 VertexPosition;
layout(location = 2) in vec2 TextureCoord;
layout(location = 3) in vec3 VertexNormal;
layout(location = 4) in ivec4 JointIndex;
layout(location = 5) in vec4 JointWeight;

const vec3 lightDir = vec3(0.1, -0.4, 0.8);
uniform mat4 GlobalPoseMatrices[100];
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 uv;
out vec3 normal;
out vec3 eyeLight;
out vec3 eyeDir;

void main()
{
	mat4 skinning = GlobalPoseMatrices[JointIndex[0]] * JointWeight[0];
	skinning += GlobalPoseMatrices[JointIndex[1]] * JointWeight[1];
	skinning += GlobalPoseMatrices[JointIndex[2]] * JointWeight[2];
	skinning += GlobalPoseMatrices[JointIndex[3]] * JointWeight[3];
	mat4 mv = V * M * skinning;
	eyeDir = normalize(-(mv * vec4(VertexPosition, 1.0)).xyz);
	eyeLight = normalize((V * vec4(lightDir, 0.0)).xyz);
	normal = normalize((mv * vec4(VertexNormal, 0.0)).xyz);
	uv = TextureCoord;
	gl_Position = P * mv * vec4(VertexPosition, 1.0);
}