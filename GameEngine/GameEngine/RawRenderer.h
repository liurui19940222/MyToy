#ifndef _RAW_RENDERER_H_
#define _RAW_RENDERER_H_

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

class ENGINE_API CRawRenderer : public CSingleton<CRawRenderer>
{
	static map<int, CSysFont*> m_fonts;
	static CRawFontRenderer m_fontRenderer;

	static CSysFont* PrepareFont(int size);
public:

	static void BeginBlend();
	static void EndBlend();
	static void SetColor(const Color& color);
	static void DrawRect(const SRect2D& rect, const Color& color);
	static void DrawString(const string& str, const Vector3& position, const Color& color, int size);
	static void DrawLine(const Vector3& p0, const Vector3& p1, const Color& color, float width);
	static void DrawPoint(const Vector3& p, const Color& color, float size);
	static void DrawTexture(CTexture& texture, const Vector3& position);
	static void DrawTexture(CTexture& texture, const SRect2D& rect);
	static void RenderString(const wstring& str, const SRect2D& rect, const Vector3& position, const Color& color, int size, EAlignment alignment);
};

#endif