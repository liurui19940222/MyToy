#version 400

layout(location = 0) out vec4 FragColor;

const int LIGHT_MAX_NUM = 12;
in vec4 eyeLight[LIGHT_MAX_NUM];
in vec4 shadowMapCoord;
in float lightNum;
in vec2 uv;
in vec3 normal;
in vec3 eyeDir;

uniform vec4 LightColor[LIGHT_MAX_NUM];
uniform float LightIntensity[LIGHT_MAX_NUM];
uniform int ShadowCasterIndex;
uniform sampler2D MainTex;
uniform sampler2D ShadowTex;
uniform float Ambient = 0.15;
uniform vec3 DiffuseColor = vec3(1.0, 1.0, 1.0);
uniform vec3 SpecularColor = vec3(1.0, 1.0, 1.0);
uniform float Gloss = 32;

void main()
{
	vec2 poissonDisk[4] = vec2[](
	  vec2( -0.94201624, -0.39906216 ),
	  vec2( 0.94558609, -0.76890725 ),
	  vec2( -0.094184101, -0.92938870 ),
	  vec2( 0.34495938, 0.29387760 )
	);
	vec3 color = texture(MainTex, uv).rgb;
	vec3 lightDir;
	vec3 halfVector;
	vec3 diffuse_total;
	vec3 specular_total;
	float ambient_avg = Ambient / lightNum;
	float cosTheta;
	float atten;
	float visibility = 1.0;

	for(int i = 0; i < lightNum; ++i)
	{
		atten = 1.0;
		if(eyeLight[i].w == 1.0)
		{
			atten = 1.0 / length(eyeLight[i].xyz);
		}
		lightDir = normalize(eyeLight[i].xyz);
		cosTheta = dot(normal, lightDir);
		diffuse_total += (max(0, cosTheta) + ambient_avg) * LightColor[i].rgb * LightIntensity[i] * atten;
		halfVector = normalize(-eyeDir + lightDir);

		if(cosTheta > 0)
		{
			specular_total += pow(max(0, dot(normal, halfVector)), Gloss) * LightColor[i].rgb * LightIntensity[i] * atten;
			if(i == ShadowCasterIndex)
			{
				float bias = 0.006*tan(acos(cosTheta));
				bias = clamp(bias, 0,0.01);
				for(int i = 0;i < 4;i++){
					if(texture2D(ShadowTex, shadowMapCoord.xy + poissonDisk[i] / 700.0 ).z  <  shadowMapCoord.z-bias ){
						visibility-=0.2;
					}
				}
				diffuse_total *= visibility;
				specular_total *= visibility;
			}
		}
	}
	color *= diffuse_total * DiffuseColor;
	color += specular_total * SpecularColor;
	FragColor = vec4(color, 1.0);
}