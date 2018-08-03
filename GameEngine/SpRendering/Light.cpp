#include"Light.h"
#include"..\SpRendering\RenderTexture.h"

USING_NAMESPACE_ENGINE

Color Light::color_buffer[LIGHT_MAX_NUM];
Vector3 Light::pos_buffer[LIGHT_MAX_NUM];
float Light::intensity_buffer[LIGHT_MAX_NUM];
int Light::type_buffer[LIGHT_MAX_NUM];
Matrix4x4 Light::shadowMapMatrix;
PRenderTexture Light::shadowMap;
vector<PLight> Light::lights;
int Light::shadowCasterIndex;

void Light::SetShadowMapSize(int width, int height)
{
	if (shadowMap.get())
		shadowMap->Release();
	shadowMap = RenderTexture::Create(1024, 1024, true);
}

PLight Light::PrepareLight()
{
	memset(color_buffer, 0, sizeof(Color) * LIGHT_MAX_NUM);
	memset(pos_buffer, 0, sizeof(Vector3) * LIGHT_MAX_NUM);
	memset(intensity_buffer, 0, sizeof(float) * LIGHT_MAX_NUM);
	memset(type_buffer, 0, sizeof(int) * LIGHT_MAX_NUM);
	PLight light;
	shadowCasterIndex = -1;
	for (uint i = 0; i < lights.size(); ++i)
	{
		color_buffer[i] = lights[i]->m_color;
		intensity_buffer[i] = lights[i]->m_intensity;
		type_buffer[i] = (int)lights[i]->m_type;
		pos_buffer[i] = lights[i]->GetLightPosOrDirection();
		if (lights[i]->IsCastShadow())
		{
			light = lights[i];
			shadowCasterIndex = i;
		}
	}
	return light;
}

PRenderTexture Light::GetShadowMap()
{
	return shadowMap;
}

Matrix4x4 Light::GetShadowMapMatrix()
{
	return shadowMapMatrix;
}

void Light::SetUniformParams(PShader shader)
{
	if (shadowMap == NULL)
		return;

	shader->SetUniformParam("LightColor", color_buffer, lights.size());
	shader->SetUniformParam("LightType", type_buffer, lights.size());
	shader->SetUniformParam("LightIntensity", intensity_buffer, lights.size());
	shader->SetUniformParam("LightDir", pos_buffer, lights.size());
	shader->SetUniformParam("LightNum", (int)lights.size());
	shader->SetUniformParam("ShadowMapTexMat", shadowMapMatrix);
	shader->SetUniformParam("ShadowCasterIndex", shadowCasterIndex);
	glActiveTexture(GL_TEXTURE1);
	shadowMap->Bind();
	shader->SetUniformParam("ShadowTex", 1);
	glActiveTexture(GL_TEXTURE0);
}
