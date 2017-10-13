#version 400

layout(location = 0) in vec3 VertexPosition;
layout(location = 2) in vec2 TextureCoord;
layout(location = 3) in vec3 VertexNormal;
const int LIGHT_MAX_NUM = 12;
uniform vec3 LightDir[LIGHT_MAX_NUM];
uniform int LightType[LIGHT_MAX_NUM];
uniform int LightNum;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 ShadowMapTexMat;

out vec4 eyeLight[LIGHT_MAX_NUM];
out vec4 shadowMapCoord;
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
		if(LightType[i] == 0)
		{
			eyeLight[i] = V * vec4(-LightDir[i], 0.0);
		}
		else if(LightType[i] == 1)
		{
			worldDir = LightDir[i] - (M * vec4(VertexPosition, 1.0)).xyz;
			eyeLight[i] = V * vec4(worldDir, 0.0);
			eyeLight[i].w = 1.0;
		}
	}
	eyeDir = normalize(mv * vec4(VertexPosition, 1.0)).xyz;
	normal = normalize((mv * vec4(VertexNormal, 0.0)).xyz);
	uv = TextureCoord;
	shadowMapCoord = ShadowMapTexMat * M * vec4(VertexPosition, 1.0);
	gl_Position = P * V * M * vec4(VertexPosition, 1.0);
}