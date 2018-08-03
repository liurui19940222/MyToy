#ifndef _SHADER_H_
#define _SHADER_H_

#include<map>
#include<string>
#include"..\ExternInclude\GL\glew.h"
#include"Math.h"
#include"..\SpCommon\EngineDefine.h"
#include"..\SpCommon\Object.hpp"

using namespace std;

BEGIN_NAMESPACE_ENGINE

enum class EShaderParamType
{
	INT = GL_INT,
	INT_VEC2 = GL_INT_VEC2,
	INT_VEC3 = GL_INT_VEC3,
	INT_VEC4 = GL_INT_VEC4,
	FLOAT = GL_FLOAT,
	FLOAT_VEC2 = GL_FLOAT_VEC2,
	FLOAT_VEC3 = GL_FLOAT_VEC3,
	FLOAT_VEC4 = GL_FLOAT_VEC4,
	BOOL = GL_BOOL,
	BOOL_VEC2 = GL_BOOL_VEC2,
	BOOL_VEC3 = GL_BOOL_VEC3,
	BOOL_VEC4 = GL_BOOL_VEC4,
	FLOAT_MAT2 = GL_FLOAT_MAT2,
	FLOAT_MAT3 = GL_FLOAT_MAT3,
	FLOAT_MAT4 = GL_FLOAT_MAT4,
	SAMPLER_1D = GL_SAMPLER_1D,
	SAMPLER_2D = GL_SAMPLER_2D,
	SAMPLER_3D = GL_SAMPLER_3D,
	SAMPLER_CUBE = GL_SAMPLER_CUBE,
	SAMPLER_1D_SHADOW = GL_SAMPLER_1D_SHADOW,
	SAMPLER_2D_SHADOW = GL_SAMPLER_2D_SHADOW,
};

SMART_CLASS(Shader) class Shader : public Object {
private:
	static map<string, PShader> m_store;

	GLuint m_program;
	GLuint m_vt;
	GLuint m_fg;
	const char* m_vtfilename;
	const char* m_fgfilename;

	Shader();
	Shader(const char* vtfilename, const char* fgfilename);

	void ShowShaderLog(GLuint shader);
	void ShowProgramLog();
	bool Compile(GLuint shader);
	bool Link();
	GLuint UniformParamLocation(const char* paramName);
	void Release();
public:
	virtual ~Shader();
	bool MakeShader(const char* vtfilename, const char* fgfilename);
	void Run();
	GLuint GetProgram();

	void SetUniformParam(const char* paramName, int value);
	void SetUniformParam(const char* paramName, float value);
	void SetUniformParam(const char* paramName, const Color& value);
	void SetUniformParam(const char* paramName, const Vector2& value);
	void SetUniformParam(const char* paramName, const Vector3& value);
	void SetUniformParam(const char* paramName, const Vector4& value);
	void SetUniformParam(const char* paramName, const Matrix4x4& value);

	void SetUniformParam(const char* paramName, int* value, int count);
	void SetUniformParam(const char* paramName, float* value, int count);
	void SetUniformParam(const char* paramName, const Color* value, int count);
	void SetUniformParam(const char* paramName, const Vector2* value, int count);
	void SetUniformParam(const char* paramName, const Vector3* value, int count);
	void SetUniformParam(const char* paramName, const Matrix4x4* value, int count);
	map<string, EShaderParamType> GetAllOfUniformParams();

	static PShader Get(const string& shaderName);
	static PShader GetDefault();
};

END_NAMESPACE_ENGINE

#endif