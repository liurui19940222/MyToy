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
		explicit GPUBuffer(const char* data, uint32_t size, uint32_t sizePerVertex, ComPtr<ID3D11Device> device, D3D11_BIND_FLAG bindBuffer, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccessFlag = 0);
		virtual ~GPUBuffer();

		void activeVertexBuffer(ComPtr<ID3D11DeviceContext> context);

	private:
		void createBuffer(const char* data, uint32_t size, ComPtr<ID3D11Device> device, D3D11_BIND_FLAG bindBuffer, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccessFlag = 0);

		uint32_t				m_SizePerVertex;

	protected:
		ComPtr<ID3D11Buffer>	m_Buffer;
	};

}