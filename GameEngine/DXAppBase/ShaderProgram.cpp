#include "ShaderProgram.h"
#include<iostream>
#include<fstream>
#include"defs.h"
#include"string_ext.h"

using namespace dxgame;

ShaderBuffer::ShaderBuffer() : bytes(nullptr), size(0)
{
	
}

ShaderBuffer::~ShaderBuffer()
{
	delete[] bytes;
}

void ShaderBuffer::resize(uint32_t size)
{
	if (bytes) delete[] bytes;
	bytes = new char[size];
	this->size = size;
}

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::ShaderProgram(const string& vertPath, const string& fragPath, ComPtr<ID3D11Device> device)
{
	LoadShader(vertPath, fragPath);
	CreateProgram(device);
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::LoadShader(const string& vertPath, const string& fragPath)
{
	if (!LoadFromFile(vertPath, m_VertexBuffer))
		throw_and_log(format_str("load vertex shader was failed. please check the path:%s", vertPath.c_str()).c_str());

	if (!LoadFromFile(fragPath, m_FragmentBuffer))
		throw_and_log(format_str("load fragment shader was failed. please check the path:%s", fragPath.c_str()).c_str());
}

void ShaderProgram::CreateProgram(ComPtr<ID3D11Device> device)
{
	if (FAILED(device->CreateVertexShader(m_VertexBuffer.bytes, m_VertexBuffer.size, nullptr, &m_VertexProgram)))
	{
		throw_and_log("create vertex program was failed.");
	}

	if (FAILED(device->CreatePixelShader(m_FragmentBuffer.bytes, m_FragmentBuffer.size, nullptr, &m_FragmentProgram)))
	{
		throw_and_log("create fragment program was failed.");
	}
}

void ShaderProgram::ActiveProgram(ComPtr<ID3D11DeviceContext> context)
{
	context->VSSetShader(m_VertexProgram.Get(), nullptr, 0);
	context->PSSetShader(m_FragmentProgram.Get(), nullptr, 0);
	context->IASetInputLayout(m_InputLayout.Get());
}

void ShaderProgram::CreateLayout(ComPtr<ID3D11Device> device, vector<D3D11_INPUT_ELEMENT_DESC>& pInputElementDescs)
{
	if (FAILED(device->CreateInputLayout(&pInputElementDescs[0], pInputElementDescs.size(), m_VertexBuffer.bytes, m_VertexBuffer.size, &m_InputLayout)))
	{
		throw_and_log("create layout was failed.");
	}
}

bool ShaderProgram::LoadFromFile(const string& path, ShaderBuffer& outBuffer)
{
	std::ifstream csoFile(path, std::ios::in | std::ios::binary | std::ios::ate);
	if (!csoFile.is_open())
		return false;
	outBuffer.resize(csoFile.tellg());
	csoFile.seekg(0, std::ios::beg);
	csoFile.read(outBuffer.bytes, outBuffer.size);
	csoFile.close();
	return true;
}