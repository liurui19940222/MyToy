#pragma once

#include<d3d11.h>
#include<wrl\client.h>
#include<vector>
#include<memory>

using namespace std;
using namespace Microsoft::WRL;

namespace dxgame {

	class GPUBuffer
	{
	public:
		explicit GPUBuffer(const char* data, uint32_t size, uint32_t sizePerVertex, ComPtr<ID3D11Device> device);
		virtual ~GPUBuffer();

		void activeBuffer(ComPtr<ID3D11DeviceContext> context);

	private:
		void createBuffer(const char* data, uint32_t size, ComPtr<ID3D11Device> device);

		ComPtr<ID3D11Buffer>	m_Buffer;
		uint32_t				m_SizePerVertex;
	};

}