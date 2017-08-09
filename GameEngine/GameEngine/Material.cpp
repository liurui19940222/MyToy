#include"Material.h"
#include"Maker.h"
#include"Texture2D.h"

IMPL_CLASS(CMaterial)

map<EPiplelineStateType, bool> CMaterial::m_pushStates;
CMaterial* CMaterial::m_defaultMaterial = NULL;

static CMaterial* get()
{
	return NULL;
}

void CMaterial::OnInitialize()
{
	Object::OnInitialize();
	m_shader = CShader::GetDefault();
	m_mainTexture = CTexture2D::GetOneInStore(EStoreTexture2DId::White8x8);

	SetState(EPiplelineStateType::AlphaTest, false);
	SetState(EPiplelineStateType::DepthTest, true);
	SetState(EPiplelineStateType::Blend, true);
	SetState(EPiplelineStateType::CullFace, false);
	SetState(EPiplelineStateType::Texture2D, true);
	SetState(EPiplelineStateType::Fog, false);
}

void CMaterial::SaveState(EPiplelineStateType state)
{
	GLboolean bstate;
	glGetBooleanv((GLenum)state, &bstate);
	SetState(m_pushStates, state, (bool)bstate);
}

void CMaterial::ApplyStates(map<EPiplelineStateType, bool>& states)
{
	auto it = states.begin();
	while (it != states.end())
	{
		if (it->second)
			glEnable((GLenum)it->first);
		else
			glDisable((GLenum)it->first);
		it++;
	}
}

void CMaterial::PushState()
{
	SaveState(EPiplelineStateType::AlphaTest);
	SaveState(EPiplelineStateType::DepthTest);
	SaveState(EPiplelineStateType::Blend);
	SaveState(EPiplelineStateType::CullFace);
	SaveState(EPiplelineStateType::Texture2D);
	SaveState(EPiplelineStateType::Fog);
	ApplyStates(m_states);
}

void CMaterial::PopState()
{
	ApplyStates(m_pushStates);
}

bool CMaterial::HasState(EPiplelineStateType state)
{
	auto it = m_states.find(state);
	if (it != m_states.end())
		return it->second;
	return false;
}

CMaterial* CMaterial::SetState(map<EPiplelineStateType, bool>& states, EPiplelineStateType state, bool open)
{
	if (states.find(state) != states.end())
		states[state] = open;
	else
		states.insert(make_pair(state, open));
	return this;
}

CMaterial* CMaterial::SetState(EPiplelineStateType state, bool open)
{
	return SetState(m_states, state, open);
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
	PushState();
	m_shader->Run();
	if (m_mainTexture && HasState(EPiplelineStateType::Texture2D))
	{
		glEnable(GL_TEXTURE_2D);
		m_mainTexture->Bind();
		SetParam("MainTex", 0);
	}
	SetParam("Color", m_color);
	return this;
}

CMaterial* CMaterial::Unbind()
{
	glUseProgram(0);
	glBindVertexArray(0);
	if (m_mainTexture && HasState(EPiplelineStateType::Texture2D))
	{
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	PopState();
	return this;
}

CMaterial* CMaterial::GetDefaltMaterial()
{
	if (m_defaultMaterial == NULL)
	{
		m_defaultMaterial = _Maker->Instantiate<CMaterial>();
	}
	return m_defaultMaterial;
}