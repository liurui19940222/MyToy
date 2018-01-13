#pragma once

#include"Rendering.h"

class GLRenderingStartUpParams : public RenderingStartUpParams
{
public:
	HWND m_HWND;
	int m_ScreenWidth;
	int m_ScreenHeight;
};

class GLRendering : public IRenderingInterface
{
public:
	virtual void StartUp(const RenderingStartUpParams* params) override;
	virtual void Render(const RenderingObject* obj, Matrix4x4& modelMatrix,
		Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix) override;
	virtual void MakeRenderContext() override;
	virtual void ShutDown() override;

	inline HDC GetHDC() { return m_HDC; }

private:
	void SetupPixelFormat(HDC hDC);

	HDC m_HDC;
	HGLRC m_HRC;
	HWND m_HWND;
};

