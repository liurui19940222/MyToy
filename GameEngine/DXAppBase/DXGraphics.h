#pragma once

#include<wrl\client.h>
#include<d3d11.h>
#include<dxgi.h>
#include<memory>
#include"D2DGraphics.h"
#include"Window.h"
#include"ShaderProgram.h"
#include"GPUBuffer.h"

using namespace Microsoft::WRL;
using namespace std;

namespace dxgame {

	struct Vertex {
		float x, y, z;
	};

	class DXGraphics
	{
	public:
		explicit DXGraphics();
		virtual ~DXGraphics();

		void init(shared_ptr<Window> window);
		void shutdown();
		void clearBuffers();
		void render();
		void present();
		void resize(int width, int height);

	private:
		void createResources(HWND hwnd);

		ComPtr<ID3D11Device>			m_D3D11Device;
		ComPtr<ID3D11DeviceContext>		m_D3D11DeviceContext;
		ComPtr<IDXGISwapChain>			m_SwapShain;
		ComPtr<ID3D11RenderTargetView>	m_RenderTargetView;
		ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;
		shared_ptr<Window>				m_Window;
		shared_ptr<ShaderProgram>		m_ShaderProgram;
		shared_ptr<GPUBuffer>			m_GPUBuffer;

		D3D11_VIEWPORT					m_Viewport;
		std::shared_ptr<D2DGraphics>	m_D2DGraphics;
	};

}