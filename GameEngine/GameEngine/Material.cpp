#include "Material.h"

IMPL_CLASS(CMaterial)

void CMaterial::OnInitialize()
{
	Object::OnInitialize();
	m_shader = CShader::GetDefault();
}

CMaterial* CMaterial::SetColor(const Color& color)
{
	m_color = color;
	return this;
}

CMaterial* CMaterial::SetShader(CShader* shader)
{
	m_shader = shader;
	return this;
}

CMaterial* CMaterial::SetMainTexture(CTexture* texture)
{
	m_mainTexture = texture;
	return this;
}

CMaterial* CMaterial::Bind()
{
	if (m_mainTexture)
	{
		m_mainTexture->Bind();
		SetParam("MainTex", 0);
	}
	SetParam("Color", m_color);
	m_shader->Run();
	return this;
}