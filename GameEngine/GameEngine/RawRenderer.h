#ifndef _RAW_RENDERER_H_
#define _RAW_RENDERER_H_

#include<Windows.h>
#include<gl\glew.h>
#include"FontManager.h"
#include"Math.h"
#include"Texture.h"
#include"Singleton.h"
#include"ApiDefine.h"
#include"SysFont.h"
#include"FontRenderer.h"

class ENGINE_API CRawFontRenderer : public CFontRenderer
{
	virtual float GetPixelScale() override;
public:
	CRawFontRenderer();
};

enum class ENGINE_API ERenderMode
{
	Wireframe,
	Fill,
};

class ENGINE_API CRawRenderer
{
	CRawFontRenderer m_fontRenderer;
	float m_width;
	float m_height;
	HWND m_hwnd;
	HDC m_hdc;
	HGLRC m_hrc;
	bool m_initialized;
	ERenderMode m_renderMode;

	void SetupPixelFormat();
public:
	CRawRenderer();

	void SetRenderMode(ERenderMode mode);
	void SetupRenderContext(HWND hwnd);
	void SetupProjection(float width, float height);
	void BeginOrtho();
	void EndOrtho();
	void BeginBlend();
	void EndBlend();
	void SetColor(const Color& color);
	void DrawMesh(const Vector3* vertices, int count, Matrix4x4& modelMatrix, const Color& color);
	void DrawRect(const SRect2D& rect, const Color& color);
	void DrawLine(const Vector3& p0, const Vector3& p1, const Color& color, float width);
	void DrawPoint(const Vector3& p, const Color& color, float size);
	void DrawTexture(CTexture& texture, const Vector3& position);
	void DrawTexture(CTexture& texture, const SRect2D& rect);
	void DrawString(const wstring& str, const SRect2D& rect, const Vector3& position, const Color& color, int size, EAlignment alignment, bool singleLine = true);
	void EnableClip(SRect2D& rect);
	void DisableClip();
	CTextOneLineData* GetLineData(int rowIndex);
	void Release();

	HDC GetDcHandle();
	HWND GetWindowHandle();
	bool Initialized();
};

#endif