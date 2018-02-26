#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include<map>
#include<gl\glew.h>
#include"Shader.h"
#include"Texture.h"
#include"SpCommon\Object.hpp"

using namespace std;

BEGIN_NAMESPACE_ENGINE

namespace statetype
{
	enum EPiplelineStateType
	{
		Texture2D = 0,
		DepthTest = 1,
		AlphaTest = 2,
		CullFace = 3,
		Blend = 4,
		Fog = 5,
		Count,
	};

	extern GLenum type[EPiplelineStateType::Count];
}

enum class EBlendFactor
{
	SRC_COLOR = GL_SRC_COLOR,
	ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
	SRC_ALPHA = GL_SRC_ALPHA,
	ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
	DST_ALPHA = GL_DST_ALPHA,
	ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
	DST_COLOR = GL_DST_COLOR,
	ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
	SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE,
};

SMART_CLASS(Material) class Material : public Object
{
private:
	virtual void OnInitialize() override;
	void SaveState(statetype::EPiplelineStateType state);
	void ApplyStates(bool states[statetype::Count]);
	Material* SetStateWithArray(bool states[statetype::Count], statetype::EPiplelineStateType state, bool open);
	void PushState();
	void PopState();
	bool HasState(statetype::EPiplelineStateType state);
public:
	Material();

	Material* SetState(statetype::EPiplelineStateType state, bool open);
	Material* SetColor(const Color& color);
	Material* SetShader(PShader shader);
	Material* SetMainTexture(PTexture texture);
	Material* SetBlendFunc(EBlendFactor src, EBlendFactor dst);
	PTexture GetMainTexture() const;
	void Bind();
	void BindTexture(const char* paramName, uint texture, int pass = 0);
	void Unbind();

	template<typename T>
	inline Material& SetParam(const char* paramName, T t)
	{
		if (m_Shader) m_Shader->SetUniformParam(paramName, t);
		return *this;
	}

	inline Material& SetParam(const char* paramName, const Matrix4x4* matrices, int count)
	{
		if (m_Shader) m_Shader->SetUniformParam(paramName, matrices, count);
		return *this;
	}

	static PMaterial GetDefaltMaterial();
private:
	static PMaterial	m_DefaultMaterial;
	static bool			m_PushStates[statetype::Count];
	Color				m_Color = Color::white;
	PShader				m_Shader = 0;
	PTexture			m_MainTexture = 0;
	bool				m_States[statetype::Count];
};

END_NAMESPACE_ENGINE

#endif