#version 400

layout(location = 0) out vec4 FragColor;
in vec2 uv;
in vec3 normal;
const int MAX_LIGHT_NUM = 2;
in vec3 eyeLight[MAX_LIGHT_NUM];
in vec3 eyeDir[MAX_LIGHT_NUM];

const float ambient = 0.1;
const vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
const vec3 specularColor = vec3(0.2, 1, 0.8);

uniform sampler2D mainTex;

void main()
{
	vec3 color = texture(mainTex, uv).rgb;
	vec3 diffuse, specular;
	for(int i = 0; i < 2; i++)
	{
		diffuse += (max(0, dot(normal, eyeLight[i])) + ambient) * diffuseColor;
		vec3 halfVector = normalize(eyeLight[i] + eyeDir[i]);
		specular += pow(max(0, dot(normal, halfVector)), 128) * specularColor;
	}
	color *= diffuse;
	color += specular;
	FragColor = vec4(color, 1.0);
}