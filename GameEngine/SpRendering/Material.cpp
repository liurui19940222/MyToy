#include"Material.h"
#include"Texture2D.h"
#include"Light.h"
#include"SpCommon\EngineDefine.h"
#include"SpCommon\Debug.h"

USING_NAMESPACE_ENGINE;

IMPL_RTTI(Material, Object::GetMetadata(), {
	PROP(Material, m_BlendFactorA, EType::Int32)
	PROP(Material, m_BlendFactorB, EType::Int32)
	PROP_ARR(Material, Bool, m_States, EType::Bool)
	PROP_ARR(Material, Float, m_Color, EType::Float)
	PROP_SHARED_PTR_CLS(Material, Shader, m_Shader)
	PROP_SHARED_PTR_CLS(Material, Texture, m_MainTexture)
})

GLenum statetype::type[] = {
	GL_TEXTURE_2D,
	GL_DEPTH_TEST,
	GL_ALPHA_TEST,
	GL_CULL_FACE,
	GL_BLEND,
	GL_FOG,
	GL_DEPTH_WRITEMASK,
};

bool Material::m_PushStates[statetype::Count];
PMaterial Material::m_DefaultMaterial = NULL;

inline void glFuncEnable(GLenum typ, GLboolean enable)
{
	if (enable)
		glEnable(typ);
	else
		glDisable(typ);
}

Material::Material() : m_BlendFactorA(EBlendFactor::SRC_ALPHA), m_BlendFactorB(EBlendFactor::ONE_MINUS_SRC_ALPHA)
{
	m_Shader = Shader::GetDefault();
	m_MainTexture = Texture2D::GetOneInStore(EStoreTexture2DId::White8x8);

	SetState(statetype::AlphaTest, false);
	SetState(statetype::DepthTest, true);
	SetState(statetype::Blend, true);
	SetState(statetype::CullFace, false);
	SetState(statetype::Texture2D, true);
	SetState(statetype::Fog, false);
	SetState(statetype::ZWrite, true);
}

void Material::OnInitialize()
{

}

void Material::SaveState(statetype::EPiplelineStateType state)
{
	if (state == statetype::ZWrite)
		return;
	GLboolean bstate = 0;
	glGetBooleanv(statetype::type[state], &bstate);
	SetStateWithArray(m_PushStates, state, _ToCppBool(bstate));
}

void Material::ApplyStates(bool states[statetype::Count])
{
	glFuncEnable(statetype::type[statetype::Texture2D], states[statetype::Texture2D]);
	glFuncEnable(statetype::type[statetype::DepthTest], states[statetype::DepthTest]);
	glFuncEnable(statetype::type[statetype::AlphaTest], states[statetype::AlphaTest]);
	glFuncEnable(statetype::type[statetype::CullFace], states[statetype::CullFace]);
	glFuncEnable(statetype::type[statetype::Blend], states[statetype::Blend]);
	glFuncEnable(statetype::type[statetype::Fog], states[statetype::Fog]);
	glDepthMask((GLboolean)states[statetype::ZWrite]);
}

void Material::PushState()
{
	SaveState(statetype::AlphaTest);
	SaveState(statetype::DepthTest);
	SaveState(statetype::Blend);
	SaveState(statetype::CullFace);
	SaveState(statetype::Texture2D);
	SaveState(statetype::Fog);
	SaveState(statetype::ZWrite);
	ApplyStates(m_States);
}

void Material::PopState()
{
	ApplyStates(m_PushStates);
}

bool Material::HasState(statetype::EPiplelineStateType state)
{
	return m_States[state];
}

Material* Material::SetStateWithArray(bool states[statetype::Count], statetype::EPiplelineStateType state, bool open)
{
	states[state] = open;
	return this;
}

Material* Material::SetState(statetype::EPiplelineStateType state, bool open)
{
	return SetStateWithArray(m_States, state, open);
}

Material* Material::SetColor(const Color& color)
{
	m_Color = color;
	return this;
}

Material* Material::SetShader(PShader shader)
{
	m_Shader = shader;
	return this;
}

Material* Material::SetMainTexture(PTexture texture)
{
	m_MainTexture = texture;
	return this;
}

Material* Material::SetBlendFunc(EBlendFactor src, EBlendFactor dst)
{
	m_BlendFactorA = src;
	m_BlendFactorB = dst;
	return this;
}

PTexture Material::GetMainTexture() const
{
	return m_MainTexture;
}

void Material::Bind()
{
	PushState();
	m_Shader->Run();
	if (m_MainTexture && HasState(statetype::Texture2D))
	{
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		m_MainTexture->Bind();
		SetParam("MainTex", 0);
	}
	SetParam("Color", m_Color);
	Light::SetUniformParams(m_Shader);
	glBlendFunc((GLenum)m_BlendFactorA, (GLenum)m_BlendFactorB);
}

void Material::BindTexture(const char* paramName, uint texture, int pass)
{
	glActiveTexture(GL_TEXTURE0 + pass);
	glBindTexture(GL_TEXTURE_2D, texture);
	SetParam(paramName, pass);
}

void Material::Unbind()
{
	glBindVertexArray(0);
	if (m_MainTexture && HasState(statetype::Texture2D))
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
	glUseProgram(0);
	PopState();
}

PMaterial Material::GetDefaltMaterial()
{
	if (m_DefaultMaterial.get() == NULL)
	{
		m_DefaultMaterial.reset(new Material());
	}
	return m_DefaultMaterial;
}