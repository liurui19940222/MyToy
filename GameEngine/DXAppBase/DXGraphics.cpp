#include "DXGraphics.h"
#include "..\SpCommon\Debug.h"
#include "defs.h"
#include <exception>

using namespace dxgame;

#pragma comment(lib, "d3d11.lib")

DXGraphics::DXGraphics()
{
}


DXGraphics::~DXGraphics()
{
}

void DXGraphics::init(shared_ptr<Window> window)
{
	HRESULT result;
	D3D_FEATURE_LEVEL featureLevel;
	unsigned int createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
		nullptr, 0, D3D11_SDK_VERSION, &m_D3D11Device, &featureLevel, &m_D3D11DeviceContext);
	if (FAILED(result))
	{
		throw_and_log("d3d11 create device was failed.");
	}

	if (featureLevel < D3D_FEATURE_LEVEL_11_0)
	{
		throw_and_log("d3d11 is not supported by your gpu.");
	}

	createResources(window->getHwnd());

	m_D2DGraphics = make_shared<D2DGraphics>();
	try
	{
		ComPtr<IDXGIDevice> dxgiDevice;
		m_D3D11Device.Get()->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice);

		ComPtr<IDXGISurface> dxgiSurface;
		m_SwapShain->GetBuffer(0, __uuidof(IDXGISurface), &dxgiSurface);
		m_D2DGraphics->init(dxgiDevice, dxgiSurface);

		m_D2DGraphics->resize(window->getWidth(), window->getHeight());
	}
	catch (std::exception ex)
	{
		spgameengine::Debug::Log(ex.what());
	}

	spgameengine::Debug::Log("d3d11 init was successful.");
}

void DXGraphics::createResources(HWND hwnd)
{
	ComPtr<IDXGIDevice>		dxgiDevice;
	ComPtr<IDXGIAdapter>	dxgiAdapter;
	ComPtr<IDXGIFactory>	dxgiFactory;

	if (FAILED(m_D3D11Device.As(&dxgiDevice)))
	{
		throw_and_log("tried get dxgidevice was failed.");
	}

	if (FAILED(dxgiDevice->GetAdapter(&dxgiAdapter)))
	{
		throw_and_log("tried get dxgiadapter was failed.");
	}

	if (FAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), &dxgiFactory)))
	{
		throw_and_log("tried get dxgifactory was failed.");
	}

	DXGI_SWAP_CHAIN_DESC swapDesc;
	swapDesc.BufferCount = 3;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.OutputWindow = hwnd;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapDesc.Windowed = true;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.BufferDesc.Width = 0;
	swapDesc.BufferDesc.Height = 0;
	swapDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	if (FAILED(dxgiFactory->CreateSwapChain(m_D3D11Device.Get(), &swapDesc, &m_SwapShain)))
	{
		throw_and_log("create swapchain was failed.");
	}

	resize(0, 0);

	// init shader
	m_ShaderProgram = make_shared<ShaderProgram>("../Debug/VertexShader.cso", "../Debug/PixelShader.cso", m_D3D11Device);
	vector<D3D11_INPUT_ELEMENT_DESC> inputDescs = { { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } };
	m_ShaderProgram->CreateLayout(m_D3D11Device, inputDescs);

	// init buffer
	Vertex mesh[3] = { { 0.0f, 1.0f, 0.3f },{ 1.0f, -1.0f, 0.3f },{ -1.0f, -1.0f, 0.3f } };
	m_GPUBuffer = make_shared<GPUBuffer>((char*)mesh, sizeof(mesh), sizeof(Vertex), m_D3D11Device);
}

void DXGraphics::shutdown()
{
	m_D2DGraphics->shutdown();
	spgameengine::Debug::Log("d3d11 shutdown.");
}

void DXGraphics::clearBuffers()
{
	m_D3D11DeviceContext->ClearState();
	float color[4] = { 0.1f, 0.0f, 0.0f, 0.0f };
	m_D3D11DeviceContext->RSSetViewports(1, &m_Viewport);
	m_D3D11DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
	m_D3D11DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
	m_D3D11DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_D2DGraphics->clearBuffers();
}

void DXGraphics::render()
{
	//m_D2DGraphics->render();

	m_GPUBuffer->activeBuffer(m_D3D11DeviceContext);
	m_ShaderProgram->ActiveProgram(m_D3D11DeviceContext);
	m_D3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_D3D11DeviceContext->Draw(3, 0);
}

void DXGraphics::present()
{
	if (FAILED(m_SwapShain->Present(0, DXGI_PRESENT_DO_NOT_WAIT)))
	{
		throw_and_log("swapchain->present was failed.");
	}

	m_D3D11DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
}

void DXGraphics::resize(int width, int height)
{
	// the both view must be release before swapchain resize 
	m_RenderTargetView = nullptr;
	m_DepthStencilView = nullptr;
	if (m_D2DGraphics)
		m_D2DGraphics->destroyRenderTarget();

	if (FAILED(m_SwapShain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0)))
	{
		throw_and_log("swapchain->resizeBuffers was failed.");
	}

	ComPtr<ID3D11Texture2D> backBuffer;
	if (FAILED(m_SwapShain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
	{
		throw_and_log("swapchain->getBuffer was failed.");
	}

	if (FAILED(m_D3D11Device->CreateRenderTargetView(backBuffer.Get(), NULL, &m_RenderTargetView)))
	{
		throw_and_log("create render target view was failed.");
	}

	ComPtr<ID3D11Texture2D> dsBuffer;
	D3D11_TEXTURE2D_DESC dsTexDesc;
	backBuffer->GetDesc(&dsTexDesc);
	dsTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsTexDesc.Usage = D3D11_USAGE_DEFAULT;
	dsTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	if (FAILED(m_D3D11Device->CreateTexture2D(&dsTexDesc, NULL, dsBuffer.GetAddressOf())))
	{
		throw_and_log("create depth and stencil texture2d was failed.");
	}

	if (FAILED(m_D3D11Device->CreateDepthStencilView(dsBuffer.Get(), NULL, &m_DepthStencilView)))
	{
		throw_and_log("create depth stencil view was failed.");
	}

	m_Viewport.Width = dsTexDesc.Width;
	m_Viewport.Height = dsTexDesc.Height;
	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_D3D11DeviceContext->RSSetViewports(1, &m_Viewport);

	m_D3D11DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

	if (m_D2DGraphics)
	{
		ComPtr<IDXGISurface> dxgiSurface;
		m_SwapShain->GetBuffer(0, __uuidof(IDXGISurface), &dxgiSurface);
		m_D2DGraphics->createRenderTarget(dxgiSurface);
		m_D2DGraphics->resize(width, height);
	}
}