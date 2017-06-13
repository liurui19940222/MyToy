#ifndef _TEXT_RENDERER_H_
#define _TEXT_RENDERER_H_

#include"Component.h"
#include"FontManager.h"
#include"EngineDefine.h"

class CCharacterPrimitive
{
public:
	int left;
	int top;
	int advance_x;

	uint32 texId;
	int width;
	int height;
	float width_x;
	float height_y;
	uint32* pixels;

	CCharacterPrimitive(int left_padding, int top, int advance_x, int width, int height, uint32* pixels);

	void Render(Vector3 pos, Vector3 size);

	void Release();
};

class CTextRenderer : public CComponent, public DynamicCreate<CTextRenderer>
{
private:
	const wchar_t* text;
	CTrueTypeFont* font;
	float interval_x;
	float interval_y;
	int font_size;
	Color color;
	SRect2D rect;
	EAlignment alignment;
	vector<CCharacterPrimitive*> primitives;

	void ClearPrimitive();

public:
	CTextRenderer();
	~CTextRenderer();

	virtual void OnStart() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnDestroy() override;

	void SetFont(CTrueTypeFont* font);

	CTrueTypeFont* GetFont();

	void SetText(const wchar_t* text);

	const wchar_t* GetText();

	void SetIntervalX(float x);

	void SetIntervalY(float y);

	void SetFontSize(int size);

	void SetColor(Color color);

	void SetAlignment(EAlignment alignment);

	void SetRect(SRect2D rect);

	void Rebuild();

	void Init(CTrueTypeFont* font, const wchar_t* text, int font_size, float interval_x, float interval_y, Color color, EAlignment alignment, SRect2D rect);
};

#endif