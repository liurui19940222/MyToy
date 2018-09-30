#pragma once

#include"GPUBuffer.h"

namespace dxgame {

	class ConstantBuffer : public GPUBuffer
	{
	public:
		ConstantBuffer(uint32_t size, ComPtr<ID3D11Device> device);

		void activeConstantBuffer(ComPtr<ID3D11DeviceContext> context);

		template<typename T>
		void setUniformParams(T data, ComPtr<ID3D11DeviceContext> context)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			context->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			memcpy_s(mappedResource.pData, sizeof(T), (void*)&data, sizeof(T));

			context->Unmap(m_Buffer.Get(), 0);
		}
	};

}