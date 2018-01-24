#include"Material.h"
#include"Texture2D.h"
#include"Light.h"
#include"SpCommon\EngineDefine.h"

using namespace spgameengine;

map<EPiplelineStateType, bool> Material::m_pushStates;
PMaterial Material::m_defaultMaterial = NULL;

Material::Material()
{
	m_shader = Shader::GetDefault();
	m_mainTexture = Texture2D::GetOneInStore(EStoreTexture2DId::White8x8);

	SetState(EPiplelineStateType::AlphaTest, false);
	SetState(EPiplelineStateType::DepthTest, true);
	SetState(EPiplelineStateType::Blend, true);
	SetState(EPiplelineStateType::CullFace, false);
	SetState(EPiplelineStateType::Texture2D, true);
	SetState(EPiplelineStateType::Fog, false);
}

void Material::OnInitialize()
{

}

void Material::SaveState(EPiplelineStateType state)
{
	GLboolean bstate;
	glGetBooleanv((GLenum)state, &bstate);
	SetState(m_pushStates, state, _ToCppBool(bstate));
}

void Material::ApplyStates(map<EPiplelineStateType, bool>& states)
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

void Material::PushState()
{
	SaveState(EPiplelineStateType::AlphaTest);
	SaveState(EPiplelineStateType::DepthTest);
	SaveState(EPiplelineStateType::Blend);
	SaveState(EPiplelineStateType::CullFace);
	SaveState(EPiplelineStateType::Texture2D);
	SaveState(EPiplelineStateType::Fog);
	ApplyStates(m_states);
}

void Material::PopState()
{
	ApplyStates(m_pushStates);
}

bool Material::HasState(EPiplelineStateType state)
{
	auto it = m_states.find(state);
	if (it != m_states.end())
		return it->second;
	return false;
}

Material* Material::SetState(map<EPiplelineStateType, bool>& states, EPiplelineStateType state, bool open)
{
	if (states.find(state) != states.end())
		states[state] = open;
	else
		states.insert(make_pair(state, open));
	return this;
}

Material* Material::SetState(EPiplelineStateType state, bool open)
{
	return SetState(m_states, state, open);
}

Material* Material::SetColor(const Color& color)
{
	m_color = color;
	return this;
}

Material* Material::SetShader(PShader shader)
{
	m_shader = shader;
	return this;
}

Material* Material::SetMainTexture(PTexture texture)
{
	m_mainTexture = texture;
	return this;
}

void Material::Bind()
{
	PushState();
	m_shader->Run();
	if (m_mainTexture && HasState(EPiplelineStateType::Texture2D))
	{
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		m_mainTexture->Bind();
		SetParam("MainTex", 0);
	}
	SetParam("Color", m_color);
	Light::SetUniformParams(m_shader);
}

void Material::Unbind()
{
	glBindVertexArray(0);
	if (m_mainTexture && HasState(EPiplelineStateType::Texture2D))
	{
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glUseProgram(0);
	PopState();
}

PMaterial Material::GetDefaltMaterial()
{
	if (m_defaultMaterial.get() == NULL)
	{
		m_defaultMaterial.reset(new Material());
	}
	return m_defaultMaterial;
}