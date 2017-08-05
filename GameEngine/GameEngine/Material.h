#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include<map>
#include"Object.h"
#include"Shader.h"
#include"Texture.h"
#include"DynamicFactory.h"

using namespace std;

class CMaterial : public Object
{
	REFLECT_CLASS(CMaterial)
private:
	Color m_color = Color::white;
	CShader* m_shader = 0;
	CTexture* m_mainTexture = 0;

	virtual void OnInitialize() override;
public:
	
	CMaterial* SetColor(const Color& color);
	CMaterial* SetShader(CShader* shader);
	CMaterial* SetMainTexture(CTexture* texture);
	CMaterial* Bind();
	
	template<typename T>
	inline CMaterial* SetParam(const char* paramName, T t)
	{
		if (m_shader) m_shader->SetUniformParam(paramName, t);
		return this;
	}
};

#endif