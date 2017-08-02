#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<Windows.h>
#include"Shader.h"
#include"Debug.h"

using namespace std;

CShader::CShader() : m_vtfilename(NULL), m_fgfilename(NULL), m_program(0)
{

}

CShader::CShader(const char* vtfilename, const char* fgfilename) : CShader()
{
	MakeShader(vtfilename, fgfilename);
}

bool CShader::MakeShader(const char* vtfilename, const char* fgfilename)
{
	Release();
	m_vtfilename = vtfilename;
	m_fgfilename = fgfilename;
	if (m_vt == 0)
		m_vt = glCreateShader(GL_VERTEX_SHADER);
	ifstream vtifs(vtfilename);
	stringstream ss;
	ss << vtifs.rdbuf();
	string str(ss.str());
	const GLchar* codeArray[] = { str.c_str() };
	glShaderSource(m_vt, 1, codeArray, NULL);
	ss.clear();
	vtifs.close();
	if (!Compile(m_vt))
		return false;
	if (m_fg == 0)
		m_fg = glCreateShader(GL_FRAGMENT_SHADER);
	ifstream fgifs(fgfilename);
	stringstream ss2;
	ss2 << fgifs.rdbuf();
	string str2 = string(ss2.str());
	codeArray[0] = str2.c_str();
	glShaderSource(m_fg, 1, codeArray, NULL);
	ss.clear();
	fgifs.close();
	if (!Compile(m_fg))
		return false;
	if (!Link())
		return false;
	return true;
}

bool CShader::Compile(GLuint shader)
{
	glCompileShader(shader);
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		ShowShaderLog(shader);
		return false;
	}
	return true;
}

bool CShader::Link()
{
	if (!m_program)
		m_program = glCreateProgram();
	glAttachShader(m_program, m_vt);
	glAttachShader(m_program, m_fg);
	glLinkProgram(m_program);
	GLint status;
	glGetProgramiv(m_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		ShowProgramLog();
		return false;
	}
	return true;
}

GLuint CShader::UniformParamLocation(const char* paramName)
{
	return glGetUniformLocation(m_program, paramName);
}

void CShader::Run()
{
	if (m_program)
		glUseProgram(m_program);
}

void CShader::ShowShaderLog(GLuint shader)
{
	GLint len;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		char* log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, NULL, log);
		OutputDebugStringA(log);
		OutputDebugStringA("\n");
		char* title = NULL;
		if (shader == m_vt)
			title = "vertex shader error";
		else if (shader == m_fg)
			title = "fragment shader error";
		CDebug::Box(title);
		free(log);
	}
}

void CShader::ShowProgramLog()
{
	GLint len;
	glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		char* log = (char*)malloc(len);
		glGetProgramInfoLog(m_program, len, NULL, log);
		OutputDebugStringA(log);
		OutputDebugStringA("\n");
		CDebug::Box(log);
		free(log);
	}
}

void CShader::Release()
{
	if (m_program)
	{
		if (m_vt)
			glDetachShader(m_program, m_vt);
		if (m_fg)
			glDetachShader(m_program, m_fg);
		glDeleteProgram(m_program);
	}
	if (m_vt)
		glDeleteShader(m_vt);
	if (m_fg)
		glDeleteShader(m_fg);
}

GLuint CShader::GetProgram()
{
	return m_program;
}

void CShader::SetUniformParam(const char* paramName, int value)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniform1i(location, value);
	}
}

void CShader::SetUniformParam(const char* paramName, float value)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniform1f(location, value);
	}
}

void CShader::SetUniformParam(const char* paramName, const Vector2& value)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniform2f(location, value.x, value.y);
	}
}

void CShader::SetUniformParam(const char* paramName, const Vector3& value)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniform3f(location, value.x, value.y, value.z);
	}
}

void CShader::SetUniformParam(const char* paramName, const Matrix4x4& value)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, (float*)&value);
	}
}