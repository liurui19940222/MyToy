#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include<map>
#include<gl\glew.h>
#include"Shader.h"
#include"Texture.h"
#include"SpCommon\Object.hpp"

using namespace std;

enum class EPiplelineStateType
{
	Texture2D = GL_TEXTURE_2D,
	DepthTest = GL_DEPTH_TEST,
	AlphaTest = GL_ALPHA_TEST,
	CullFace = GL_CULL_FACE,
	Blend = GL_BLEND,
	Fog = GL_FOG,
};

SMART_CLASS(Material) class Material : public Object
{
private:
	static PMaterial m_defaultMaterial;
	static map<EPiplelineStateType, bool> m_pushStates;
	map<EPiplelineStateType, bool> m_states;
	Color m_color = Color::white;
	PShader m_shader = 0;
	PTexture m_mainTexture = 0;

	virtual void OnInitialize() override;
	void SaveState(EPiplelineStateType state);
	void ApplyStates(map<EPiplelineStateType, bool>& states);
	Material* SetState(map<EPiplelineStateType, bool>& states, EPiplelineStateType state, bool open);
	void PushState();
	void PopState();
	bool HasState(EPiplelineStateType state);
public:
	Material();

	Material* SetState(EPiplelineStateType state, bool open);
	Material* SetColor(const Color& color);
	Material* SetShader(PShader shader);
	Material* SetMainTexture(PTexture texture);
	void Bind();
	void Unbind();
	
	template<typename T>
	inline Material& SetParam(const char* paramName, T t)
	{
		if (m_shader) m_shader->SetUniformParam(paramName, t);
		return *this;
	}

	inline Material& SetParam(const char* paramName, const Matrix4x4* matrices, int count)
	{
		if (m_shader) m_shader->SetUniformParam(paramName, matrices, count);
		return *this;
	}

	static PMaterial GetDefaltMaterial();
};

#endif