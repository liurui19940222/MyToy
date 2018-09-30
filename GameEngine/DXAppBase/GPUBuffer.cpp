#include "GPUBuffer.h"
#include "defs.h"

using namespace dxgame;

GPUBuffer::GPUBuffer(const char* data, uint32_t size, uint32_t sizePerVertex, ComPtr<ID3D11Device> device, D3D11_BIND_FLAG bindBuffer, D3D11_USAGE usage, UINT cpuAccessFlag) : m_SizePerVertex(sizePerVertex)
{
	createBuffer(data, size, device, bindBuffer, usage, cpuAccessFlag);
}


GPUBuffer::~GPUBuffer()
{
	logD("buffer is releasing...");
}

void GPUBuffer::createBuffer(const char* data, uint32_t size, ComPtr<ID3D11Device> device, D3D11_BIND_FLAG bindBuffer, D3D11_USAGE usage, UINT cpuAccessFlag)
{
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = size;
	desc.Usage = usage;
	desc.BindFlags = bindBuffer;
	desc.CPUAccessFlags = cpuAccessFlag;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	if (data != nullptr)
	{
		D3D11_SUBRESOURCE_DATA sub = { &data[0], 0, 0 };

		if (FAILED(device->CreateBuffer(&desc, &sub, &m_Buffer)))
		{
			throw_and_log("create buffer was failed.");
		}
	}
	else
	{
		desc.ByteWidth = 16;

		HRESULT result;
		if (FAILED(result = device->CreateBuffer(&desc, NULL, &m_Buffer)))
		{
			throw_and_log("create buffer was failed.");
		}
	}
}

void GPUBuffer::activeVertexBuffer(ComPtr<ID3D11DeviceContext> context)
{
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, m_Buffer.GetAddressOf(), &m_SizePerVertex, &offset);
}