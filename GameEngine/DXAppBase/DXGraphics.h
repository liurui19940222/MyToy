#pragma once

#include<wrl\client.h>
#include<d3d11.h>
#include<dxgi.h>

using namespace Microsoft::WRL;

namespace dxgame {

	class DXGraphics
	{
	public:
		explicit DXGraphics();
		virtual ~DXGraphics();

		void init(HWND hwnd);
		void shutdown();
		void clearBuffers();
		void present();
		void resize(int width, int height);

	private:
		void createResources(HWND hwnd);

		ComPtr<ID3D11Device>			m_D3D11Device;
		ComPtr<ID3D11DeviceContext>		m_D3D11DeviceContext;
		ComPtr<IDXGISwapChain>			m_SwapShain;
		ComPtr<ID3D11RenderTargetView>	m_RenderTargetView;
		ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;
	};

}