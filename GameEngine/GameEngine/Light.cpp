#include"Light.h"
#include"GameObject.h"
#include"Maker.h"
#include"RenderTexture.h"

Color CLight::color_buffer[LIGHT_MAX_NUM];
Vector3 CLight::pos_buffer[LIGHT_MAX_NUM];
float CLight::intensity_buffer[LIGHT_MAX_NUM];
int CLight::type_buffer[LIGHT_MAX_NUM];
CRenderTexture* CLight::shadowMap;
vector<CLight*> CLight::lights;

void CLight::SetShadowMapSize(int width, int height)
{
	if (shadowMap)
		_Maker->Destroy(shadowMap);
	shadowMap = CRenderTexture::Create(1024, 1024, true);
}

CLight* CLight::PrepareLight()
{
	memset(color_buffer, 0, sizeof(Color) * LIGHT_MAX_NUM);
	memset(pos_buffer, 0, sizeof(Vector3) * LIGHT_MAX_NUM);
	memset(intensity_buffer, 0, sizeof(float) * LIGHT_MAX_NUM);
	memset(type_buffer, 0, sizeof(int) * LIGHT_MAX_NUM);
	CLight* light = NULL;

	for (int i = 0; i < lights.size(); ++i)
	{
		color_buffer[i] = lights[i]->m_color;
		intensity_buffer[i] = lights[i]->m_intensity;
		type_buffer[i] = (int)lights[i]->m_type;
		pos_buffer[i] = lights[i]->GetLightPosOrDirection();
		if (lights[i]->IsCastShadow())
		{
			light = lights[i];
		}
	}
	return light;
}

void CLight::SetUniformParams(CShader* shader)
{
	shader->SetUniformParam("LightColor", color_buffer, lights.size());
	shader->SetUniformParam("LightType", type_buffer, lights.size());
	shader->SetUniformParam("LightIntensity", intensity_buffer, lights.size());
	shader->SetUniformParam("LightDir", pos_buffer, lights.size());
	shader->SetUniformParam("LightNum", (int)lights.size());
}

Vector3 CLight::GetLightPosOrDirection() const
{
	if (m_type == ELightType::Directional)
		return gameObject->GetForward();
	return gameObject->GetLocalPosition();
}

void CLight::OnStart()
{
	CComponent::OnStart();
	lights.push_back(this);
}

void CLight::OnDestroy()
{
	CComponent::OnDestroy();
	for (auto it = lights.begin(); it != lights.end(); ++it)
	{
		if (*it == this)
		{
			lights.erase(it);
			return;
		}
	}
}