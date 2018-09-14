#include "GPUBuffer.h"
#include "defs.h"

using namespace dxgame;

GPUBuffer::GPUBuffer(const char* data, uint32_t size, uint32_t sizePerVertex, ComPtr<ID3D11Device> device) : m_SizePerVertex(sizePerVertex)
{
	createBuffer(data, size, device);
}


GPUBuffer::~GPUBuffer()
{
}

void GPUBuffer::createBuffer(const char* data, uint32_t size, ComPtr<ID3D11Device> device)
{
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = size;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sub = { &data[0], 0, 0 };

	if (FAILED(device->CreateBuffer(&desc, &sub, &m_Buffer)))
	{
		throw_and_log("create buffer was failed.");
	}
}

void GPUBuffer::activeBuffer(ComPtr<ID3D11DeviceContext> context)
{
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, m_Buffer.GetAddressOf(), &m_SizePerVertex, &offset);
}