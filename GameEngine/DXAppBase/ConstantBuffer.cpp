#include "ConstantBuffer.h"

using namespace dxgame;

ConstantBuffer::ConstantBuffer(uint32_t size, ComPtr<ID3D11Device> device) : GPUBuffer(nullptr, size, 0, device, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE)
{

}

void ConstantBuffer::activeConstantBuffer(ComPtr<ID3D11DeviceContext> context)
{
	int bufferNum = 0;
	context->VSSetConstantBuffers(bufferNum, 1, m_Buffer.GetAddressOf());
}