#version 400

layout(location = 0) in vec3 VertexPosition;
layout(location = 2) in vec2 TextureCoord;
layout(location = 3) in vec3 VertexNormal;
const int LIGHT_MAX_NUM = 10;
uniform vec3 LightDir[LIGHT_MAX_NUM];
uniform int LightType[LIGHT_MAX_NUM];
uniform int LightNum;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 eyeLight[LIGHT_MAX_NUM];
out float atten[LIGHT_MAX_NUM];
out float lightNum;
out vec2 uv;
out vec3 normal;
out vec3 eyeDir;

void main()
{
	lightNum = LightNum;
	mat4 mv = V * M;
	vec3 worldDir;
	for(int i = 0; i < lightNum; ++i)
	{
		//if(LightType[i] == 0)
		//{
			eyeLight[i] = normalize((V * vec4(-LightDir[i], 0.0)).xyz);
			atten[i] = 1.0;
		//}
		//else if(LightType[i] == 1)
		//{
		//	vec4 worldPos = M * vec4(VertexPosition, 1.0);
		//	worldDir = LightDir[i] - worldPos.xyz;
		//	eyeLight[i] = normalize((V * vec4(worldDir, 0.0)).xyz);
		//	atten[i] = 1.0 / length(worldDir);
		//}
	}
	eyeDir = normalize(-(mv * vec4(VertexPosition, 1.0)).xyz);
	normal = normalize((mv * vec4(VertexNormal, 0.0)).xyz);
	uv = TextureCoord;
	gl_Position = P * V * M * vec4(VertexPosition, 1.0);
}