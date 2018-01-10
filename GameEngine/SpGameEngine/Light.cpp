#include"Light.h"
#include"GameObject.h"
#include"Maker.h"
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
	shadowCasterIndex = -1;
	for (int i = 0; i < lights.size(); ++i)
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

void CLight::RenderShadowMap(CLight* light)
{
	if (!light) return;

	CRenderTexture* renderTex = CLight::GetShadowMap();
	CShader* shadowShader = CShader::Get("shadow");
	Matrix4x4 shadow_view = Matrix4x4::LookAt(-light->gameObject->GetForward(), Vector3::zero, Vector3::forward);
	Matrix4x4 shadow_projection = Matrix4x4::Ortho(-10, 10, -10, 10, -10, 20);
	shadowMapMatrix = Matrix4x4::Translate(Vector3(0.5, 0.5, 0.5)) * Matrix4x4::Scale(Vector3(0.5, 0.5, 0.5)) * shadow_projection * shadow_view;

	CLight::GetShadowMap()->BindBuffer();
	glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, renderTex->GetWidth(), renderTex->GetHeight());
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	shadowShader->Run();
	shadowShader->SetUniformParam("V", shadow_view);
	shadowShader->SetUniformParam("P", shadow_projection);

	IRenderer* renderer = NULL;
	_Maker->ForeachGameObject([&renderer, &shadowShader](CGameObject* go, int depth, Matrix4x4& mat) {
		if (go->IsActive() && (renderer = go->GetRenderer()) != NULL)
		{
			shadowShader->SetUniformParam("M", mat);
			renderer->Render(shadowShader);
		}
	});

	glPopAttrib();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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