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
}

CMaterial* CMaterial::SetShader(CShader* shader)
{
	m_shader = shader;
}

CMaterial* CMaterial::SetMainTexture(CTexture* texture)
{
	m_mainTexture = texture;
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
}