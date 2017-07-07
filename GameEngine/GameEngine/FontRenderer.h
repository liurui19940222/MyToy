#ifndef _FONT_RENDERER_H_
#define _FONT_RENDERER_H_

#include"Math.h"
#include"FontManager.h"
#include"EngineDefine.h"

class CCharacterPrimitive
{
public:
	Vector3 position;
	int left;
	int top;
	int advance_x;

	uint32 texId;
	int width;
	int height;
	float width_x;
	float height_y;
	uint32* pixels;

	CCharacterPrimitive(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels);

	void Render(Vector3 pos, Vector3 size);

	void Release();
};

class CTextOneLineData
{
public:
	CTextOneLineData();

	float line_width;

	vector<CCharacterPrimitive*> primitives;

	void Release();
};

class CFontRenderer
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
	EAlignmentHorizontal alignment_h;
	EAlignmentVertical alignment_v;
	vector<CCharacterPrimitive*> primitives;
	vector<CTextOneLineData*> lineDatas;

protected:
	void ClearPrimitive();

	void ClearLineData();

	virtual float GetPixelScale() = 0;

	void Render();

	float GetOffsetX(float line_width);

	float GetOffsetY();

	void Rebuild();

	void Init(CTrueTypeFont* font, const wchar_t* text, int font_size, float interval_x, float interval_y, Color color, EAlignment alignment, SRect2D rect);

public:
	virtual CFontRenderer* SetTextRect(SRect2D rect);

	SRect2D GetTextRect();

	CFontRenderer* SetFont(CTrueTypeFont* font);

	CTrueTypeFont* GetFont();

	CFontRenderer* SetText(const wchar_t* text);

	const wchar_t* GetText();

	CFontRenderer* SetIntervalX(float x);

	CFontRenderer* SetIntervalY(float y);

	CFontRenderer* SetFontSize(int size);

	CFontRenderer* SetColor(Color color);

	CFontRenderer* SetTextAlignment(EAlignment alignment);
};

#endif