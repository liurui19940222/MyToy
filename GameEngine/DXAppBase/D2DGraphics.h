#pragma once

#include<dxgi.h>
#include<d2d1.h>
#include<d2d1_1.h>
#include<d2d1_2.h>
#include<dwrite.h>
#include<wrl\client.h>

using namespace Microsoft::WRL;

namespace dxgame {

	class D2DGraphics
	{
	public:
		explicit D2DGraphics() = default;
		virtual ~D2DGraphics() = default;

		void init(ComPtr<IDXGIDevice> dxgiDevice, ComPtr<IDXGISurface> dxgiSurface);
		void shutdown();
		void clearBuffers();
		void present();
		void resize(int width, int height);

	private:
		void createDevice(ComPtr<IDXGIDevice> dxgiDevice);
		void createRenderTarget(ComPtr<IDXGISurface> dxgiSurface);
		void initTextFormats();

		ComPtr<IDWriteFactory>			m_WriteFactory;
		ComPtr<ID2D1Factory2>			m_D2DFactory;
		ComPtr<ID2D1Device>				m_D2DDevice;
		ComPtr<ID2D1DeviceContext>		m_D2DDeviceContext;
		
		ComPtr<ID2D1SolidColorBrush>	m_BrushWhite;
		ComPtr<ID2D1SolidColorBrush>	m_BrushBlack;
		ComPtr<ID2D1SolidColorBrush>	m_BrushYellow;

		ComPtr<IDWriteTextFormat>		m_FormatFps;
		ComPtr<IDWriteTextLayout>		m_LayoutFps;
	};

}