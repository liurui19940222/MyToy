#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include<map>
#include<gl\glew.h>
#include"Object.h"
#include"Shader.h"
#include"Texture.h"
#include"DynamicFactory.h"

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

class CMaterial : public Object
{
	REFLECT_CLASS(CMaterial)
private:
	static CMaterial* m_defaultMaterial;
	static map<EPiplelineStateType, bool> m_pushStates;
	map<EPiplelineStateType, bool> m_states;
	Color m_color = Color::white;
	CShader* m_shader = 0;
	CTexture* m_mainTexture = 0;

	virtual void OnInitialize() override;
	void SaveState(EPiplelineStateType state);
	void ApplyStates(map<EPiplelineStateType, bool>& states);
	CMaterial* SetState(map<EPiplelineStateType, bool>& states, EPiplelineStateType state, bool open);
	void PushState();
	void PopState();
	bool HasState(EPiplelineStateType state);
public:
	CMaterial* SetState(EPiplelineStateType state, bool open);
	CMaterial* SetColor(const Color& color);
	CMaterial* SetShader(CShader* shader);
	CMaterial* SetMainTexture(CTexture* texture);
	CMaterial* Bind();
	CMaterial* Unbind();
	
	template<typename T>
	inline CMaterial* SetParam(const char* paramName, T t)
	{
		if (m_shader) m_shader->SetUniformParam(paramName, t);
		return this;
	}

	inline CMaterial* SetParam(const char* paramName, const Matrix4x4* matrices, int count)
	{
		if (m_shader) m_shader->SetUniformParam(paramName, matrices, count);
		return this;
	}

	static CMaterial* GetDefaltMaterial();
};

#endif