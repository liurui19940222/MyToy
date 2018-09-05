#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<Windows.h>
#include"Shader.h"
#include "SpCommon\Debug.h"
#include"SpCommon\Converter.h"

using namespace std;
USING_NAMESPACE_ENGINE;

IMPL_RTTI(Shader, Object::GetMetadata(), {
	PROP(Shader, m_vtfilename, EType::String)
	PROP(Shader, m_fgfilename, EType::String)
})

map<string, ShaderPtr> Shader::m_store;

Shader::Shader() : Object(), m_program(0), m_vt(0), m_fg(0)
{

}

Shader::Shader(const char* vtfilename, const char* fgfilename) : Shader()
{
	MakeShader(vtfilename, fgfilename);
}

Shader::~Shader()
{
	Release();
}

bool Shader::MakeShader(const char* vtfilename, const char* fgfilename)
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

bool Shader::Compile(GLuint shader)
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

bool Shader::Link()
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

GLuint Shader::UniformParamLocation(const char* paramName)
{
	return glGetUniformLocation(m_program, paramName);
}

void Shader::Run()
{
	if (m_program)
		glUseProgram(m_program);
}

void Shader::ShowShaderLog(GLuint shader)
{
	GLint len;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		char* log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, NULL, log);
		OutputDebugStringA(log);
		OutputDebugStringA("\n");
		string title = log;
		if (shader == m_vt)
			title = "vertex shader error\n" + title;
		else if (shader == m_fg)
			title = "fragment shader error\n" + title;
		Debug::Box(title.c_str());
		std::free(log);
	}
}

void Shader::ShowProgramLog()
{
	GLint len;
	glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		char* log = (char*)malloc(len);
		glGetProgramInfoLog(m_program, len, NULL, log);
		OutputDebugStringA(log);
		OutputDebugStringA("\n");
		Debug::Box(log);
		std::free(log);
	}
}

void Shader::Release()
{
	if (m_program)
	{
		if (m_vt)
			glDetachShader(m_program, m_vt);
		if (m_fg)
			glDetachShader(m_program, m_fg);
		glDeleteProgram(m_program);
	}
	if (m_vt) glDeleteShader(m_vt);
	if (m_fg) glDeleteShader(m_fg);
	m_program = 0;
	m_vt = 0;
	m_fg = 0;
}

GLuint Shader::GetProgram()
{
	return m_program;
}

void Shader::SetUniformParam(const char* paramName, int value)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniform1i(location, value);
	}
}

void Shader::SetUniformParam(const char* paramName, float value)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniform1f(location, value);
	}
}

void Shader::SetUniformParam(const char* paramName, const Color& value)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniform4f(location, value.r, value.g, value.b, value.a);
	}
}

void Shader::SetUniformParam(const char* paramName, const Vector2& value)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniform2f(location, value.x, value.y);
	}
}

void Shader::SetUniformParam(const char* paramName, const Vector3& value)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniform3f(location, value.x, value.y, value.z);
	}
}

void Shader::SetUniformParam(const char* paramName, const Vector4& value)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
}

void Shader::SetUniformParam(const char* paramName, const Matrix4x4& value)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, (float*)&value);
	}
}

void Shader::SetUniformParam(const char* paramName, int* value, int count)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniform1iv(location, count, value);
	}
}

void Shader::SetUniformParam(const char* paramName, float* value, int count)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniform1fv(location, count, value);
	}
}

void Shader::SetUniformParam(const char* paramName, const Color* value, int count)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniform4fv(location, count, (float*)value);
	}
}

void Shader::SetUniformParam(const char* paramName, const Vector2* value, int count)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniform2fv(location, count, (float*)value);
	}
}

void Shader::SetUniformParam(const char* paramName, const Vector3* value, int count)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniform3fv(location, count, (float*)value);
	}
}

void Shader::SetUniformParam(const char* paramName, const Matrix4x4* value, int count)
{
	GLuint location = UniformParamLocation(paramName);
	if (location >= 0)
	{
		glUniformMatrix4fv(location, count, GL_FALSE, (float*)value);
	}
}

map<string, EShaderParamType> Shader::GetAllOfUniformParams()
{
	map<string, EShaderParamType> list;
	GLint maxUniformLen;
	GLint numUniforms;
	GLint index;
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &numUniforms);
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH,
		&maxUniformLen);
	char *uniformName = (char*)malloc(sizeof(char) * maxUniformLen);
	for (index = 0; index < numUniforms; index++)
	{
		string name;
		GLint size;
		GLenum type;
		glGetActiveUniform(m_program, index, maxUniformLen, NULL,
			&size, &type, uniformName);
		name = uniformName;
		list.insert(make_pair(name, (EShaderParamType)type));
	}
	std::free(uniformName);
	return list;
}

ShaderPtr Shader::Get(const string& shaderName)
{
	ShaderPtr shader;
	auto it = m_store.find(shaderName);
	if (it == m_store.end())
	{
		string vt = "../Shaders/" + shaderName + ".vert";
		string fg = "../Shaders/" + shaderName + ".frag";
		shader = make_shared<Shader>(vt.c_str(), fg.c_str());
		m_store.insert(make_pair(shaderName, shader));
	}
	else
	{
		shader = it->second;;
	}
	return shader;
}

ShaderPtr Shader::GetDefault()
{
	return Get("basic");
}

void Shader::OnSerialize(int depth, const Metadata* meta, Value& value, MemoryPoolAllocator<>& allocator)
{
	Object::OnSerialize(depth, meta, value, allocator);
}

void Shader::OnDeserialize(int depth, const Metadata* meta, Value& value)
{
	Object::OnDeserialize(depth, meta, value);
	Release();
	if (!m_vtfilename.empty() && !m_fgfilename.empty())
	{
		MakeShader(m_vtfilename.c_str(), m_fgfilename.c_str());
	}
}