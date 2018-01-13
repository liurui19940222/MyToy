#include"Light.h"
#include"..\SpRendering\RenderTexture.h"

Color CLight::color_buffer[LIGHT_MAX_NUM];
Vector3 CLight::pos_buffer[LIGHT_MAX_NUM];
float CLight::intensity_buffer[LIGHT_MAX_NUM];
int CLight::type_buffer[LIGHT_MAX_NUM];
Matrix4x4 CLight::shadowMapMatrix;
CRenderTexture* CLight::shadowMap;
vector<CLight*> CLight::lights;
int CLight::shadowCasterIndex;

void CLight::SetShadowMapSize(int width, int height)
{
	if (shadowMap)
		delete shadowMap;
	shadowMap = CRenderTexture::Create(1024, 1024, true);
}

CLight* CLight::PrepareLight()
{
	memset(color_buffer, 0, sizeof(Color) * LIGHT_MAX_NUM);
	memset(pos_buffer, 0, sizeof(Vector3) * LIGHT_MAX_NUM);
	memset(intensity_buffer, 0, sizeof(float) * LIGHT_MAX_NUM);
	memset(type_buffer, 0, sizeof(int) * LIGHT_MAX_NUM);
	CLight* light = NULL;
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

CRenderTexture* CLight::GetShadowMap()
{
	return shadowMap;
}

Matrix4x4 CLight::GetShadowMapMatrix()
{
	return shadowMapMatrix;
}

void CLight::SetUniformParams(CShader* shader)
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
