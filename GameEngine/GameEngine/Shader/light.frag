#version 400

layout(location = 0) out vec4 FragColor;

const int LIGHT_MAX_NUM = 10;
in vec3 eyeLight[LIGHT_MAX_NUM];
in float atten[LIGHT_MAX_NUM];
in float lightNum;
in vec2 uv;
in vec3 normal;
in vec3 eyeDir;

uniform vec4 LightColor[LIGHT_MAX_NUM];
uniform float LightIntensity[LIGHT_MAX_NUM];
uniform sampler2D MainTex;
uniform float Ambient = 0.15;
uniform vec3 DiffuseColor = vec3(1.0, 1.0, 1.0);
uniform vec3 SpecularColor = vec3(1.0, 1.0, 1.0);
uniform float Gloss = 8;

void main()
{
	vec3 color = texture(MainTex, uv).rgb;
	vec3 halfVector;
	vec3 diffuse_total;
	vec3 specular_total;
	float ambient_avg = Ambient / lightNum;

	for(int i = 0; i < lightNum; ++i)
	{
		diffuse_total += (max(0, dot(normal, eyeLight[i])) + ambient_avg) * LightColor[i].rgb * LightIntensity[i] * atten[i];
		halfVector = normalize(eyeLight[i] + eyeDir);
		specular_total += pow(max(0, dot(normal, halfVector)), Gloss) * LightColor[i].rgb * LightIntensity[i] * atten[i];
	}
	color *= diffuse_total * DiffuseColor;
	color += specular_total * SpecularColor;
	FragColor = vec4(color, 1.0);
}