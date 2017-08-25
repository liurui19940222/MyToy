#version 400

layout(location = 0) out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 eyeLight;
in vec3 eyeDir;

const vec3 ambientColor = vec3(0.05f, 0.05f, 0.05f);
const vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
const vec3 specularColor = vec3(0.5, 0.5, 0.6);

uniform sampler2D mainTex;

void main()
{
	vec3 color = texture(mainTex, uv).rgb;
	vec3 diffuse = max(0, dot(normal, eyeLight)) * diffuseColor;
	vec3 halfVector = normalize(eyeLight + eyeDir);
	vec3 specular = pow(max(0, dot(normal, halfVector)), 8) * specularColor;
	color *= diffuse;
	color += specular + ambientColor;
	FragColor = vec4(color, 1.0);
}