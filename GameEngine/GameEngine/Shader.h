#ifndef _SHADER_H_
#define _SHADER_H_

#include<gl\glew.h>
#include"Math.h"

class CShader
{
private:
	GLuint m_program;
	GLuint m_vt;
	GLuint m_fg;
	const char* m_vtfilename;
	const char* m_fgfilename;

	void ShowShaderLog(GLuint shader);

	void ShowProgramLog();

	bool Compile(GLuint shader);

	bool Link();

	GLuint UniformParamLocation(const char* paramName);

public:
	CShader();
	CShader(const char* vtfilename, const char* fgfilename);

	bool MakeShader(const char* vtfilename, const char* fgfilename);

	void Run();

	void Release();

	GLuint GetProgram();

	void SetUniformParam(const char* paramName, int value);
	void SetUniformParam(const char* paramName, float value);
	void SetUniformParam(const char* paramName, const Vector2& value);
	void SetUniformParam(const char* paramName, const Vector3& value);
	void SetUniformParam(const char* paramName, const Matrix4x4& value);
};

#endif