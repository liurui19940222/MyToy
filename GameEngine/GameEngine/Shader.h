#ifndef _SHADER_H_
#define _SHADER_H_

#include<map>
#include<string>
#include<gl\glew.h>
#include"Object.h"
#include"Math.h"
#include"EngineDefine.h"

using namespace std;

enum EShaderParamType
{
	T_INT = GL_INT,
	T_INT_VEC2 = GL_INT_VEC2,
	T_INT_VEC3 = GL_INT_VEC3,
	T_INT_VEC4 = GL_INT_VEC4,
	T_FLOAT =	GL_FLOAT,
	T_FLOAT_VEC2 = GL_FLOAT_VEC2,
	T_FLOAT_VEC3 = GL_FLOAT_VEC3,
	T_FLOAT_VEC4 = GL_FLOAT_VEC4,
	T_BOOL = GL_BOOL,
	T_BOOL_VEC2 = GL_BOOL_VEC2,
	T_BOOL_VEC3 = GL_BOOL_VEC3,
	T_BOOL_VEC4 = GL_BOOL_VEC4,
	T_FLOAT_MAT2 = GL_FLOAT_MAT2,
	T_FLOAT_MAT3 = GL_FLOAT_MAT3,
	T_FLOAT_MAT4 = GL_FLOAT_MAT4,
	T_SAMPLER_1D = GL_SAMPLER_1D,
	T_SAMPLER_2D = GL_SAMPLER_2D,
	T_SAMPLER_3D = GL_SAMPLER_3D,
	T_SAMPLER_CUBE = GL_SAMPLER_CUBE,
	T_SAMPLER_1D_SHADOW = GL_SAMPLER_1D_SHADOW,
	T_SAMPLER_2D_SHADOW = GL_SAMPLER_2D_SHADOW,
};

class CShader : public Object
{
private:
	static map<string, CShader*> m_store;

	GLuint m_program;
	GLuint m_vt;
	GLuint m_fg;
	const char* m_vtfilename;
	const char* m_fgfilename;

	CShader();
	CShader(const char* vtfilename, const char* fgfilename);

	void ShowShaderLog(GLuint shader);
	void ShowProgramLog();
	bool Compile(GLuint shader);
	bool Link();
	GLuint UniformParamLocation(const char* paramName);
	virtual void OnRelease() override;
public:

	bool MakeShader(const char* vtfilename, const char* fgfilename);
	void Run();
	GLuint GetProgram();

	void SetUniformParam(const char* paramName, int value);
	void SetUniformParam(const char* paramName, float value);
	void SetUniformParam(const char* paramName, const Color& value);
	void SetUniformParam(const char* paramName, const Vector2& value);
	void SetUniformParam(const char* paramName, const Vector3& value);
	void SetUniformParam(const char* paramName, const Matrix4x4& value);
	map<string, EShaderParamType> GetAllOfUniformParams();

	static CShader* Get(const string& shaderName);
	static CShader* GetDefault();
};

#endif