#ifndef _SHADER_H_
#define _SHADER_H_

#include<map>
#include<string>
#include<gl\glew.h>
#include"Math.h"
#include"EngineDefine.h"

using namespace std;

class CShader
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

public:

	bool MakeShader(const char* vtfilename, const char* fgfilename);
	void Run();
	void Release();
	GLuint GetProgram();

	void SetUniformParam(const char* paramName, int value);
	void SetUniformParam(const char* paramName, float value);
	void SetUniformParam(const char* paramName, const Color& value);
	void SetUniformParam(const char* paramName, const Vector2& value);
	void SetUniformParam(const char* paramName, const Vector3& value);
	void SetUniformParam(const char* paramName, const Matrix4x4& value);

	static CShader* Get(const string& shaderName);
};

#endif