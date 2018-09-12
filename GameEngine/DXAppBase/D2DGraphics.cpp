#include "D2DGraphics.h"
#include "defs.h"
#include "../SpCommon/Debug.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

using namespace dxgame;

void D2DGraphics::init(ComPtr<IDXGIDevice> dxgiDevice, ComPtr<IDXGISurface> dxgiSurface)
{
	createDevice(dxgiDevice);
	createRenderTarget(dxgiSurface);
	initTextFormats();
}

void D2DGraphics::shutdown()
{

}

void D2DGraphics::clearBuffers()
{

}

void D2DGraphics::present()
{
	
}

void D2DGraphics::resize(int width, int height)
{
	
}

void D2DGraphics::createDevice(ComPtr<IDXGIDevice> dxgiDevice)
{
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &m_WriteFactory)))
	{
		throw_and_log("create writefactory was failed.");
	}

	D2D1_FACTORY_OPTIONS options;
#ifdef _DEBUG
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
	options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif

	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory), &options, &m_D2DFactory)))
	{
		throw_and_log("create d2d factory was failed.");
	}

	if (FAILED(m_D2DFactory->CreateDevice(dxgiDevice.Get(), &m_D2DDevice)))
	{
		throw_and_log("create d2d device was failed.");
	}

	if (FAILED(m_D2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &m_D2DDeviceContext)))
	{
		throw_and_log("create d2d device context was failed.");
	}
}

void D2DGraphics::createRenderTarget(ComPtr<IDXGISurface> dxgiSurface)
{
	D2D1_BITMAP_PROPERTIES1 prop;
	prop.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	prop.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	prop.dpiX = 96.0f;
	prop.dpiY = 96.0f;
	prop.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	prop.colorContext = nullptr;

	ComPtr<ID2D1Bitmap1> targetBitmap;
	if (FAILED(m_D2DDeviceContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), prop, &targetBitmap)))
	{
		throw_and_log("create d2d bitmap was failed.");
	}

	m_D2DDeviceContext->SetTarget(targetBitmap.Get());
}

void D2DGraphics::initTextFormats()
{
	m_D2DDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &m_BrushYellow);
	m_D2DDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_BrushWhite);
	m_D2DDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_BrushBlack);

	m_WriteFactory->CreateTextFormat(L"", nullptr, DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STYLE_NORMAL, 
		DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"en-GB", &m_FormatFps);

	m_FormatFps->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
}