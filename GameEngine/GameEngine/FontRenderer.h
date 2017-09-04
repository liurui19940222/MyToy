#ifndef _FONT_RENDERER_H_
#define _FONT_RENDERER_H_

#include"Math.h"
#include"FontManager.h"
#include"EngineDefine.h"
#include"Renderer.h"
#include"MeshBuffer.h"
#include"Texture2D.h"
#include"Material.h"

#define TEXT_BUFFER_SIZE 2048

class CCharacterPrimitive
{
public:
	CMeshBuffer m_buffer;
	CMaterial* m_material;
	CTexture2D* m_texture;
	Vector3 position;
	int left;
	int top;
	int advance_x;
	int width;
	int height;
	float width_x;
	float height_y;

	CCharacterPrimitive(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels);
	~CCharacterPrimitive();
	void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color);
};

class CTextOneLineData
{
public:
	CTextOneLineData();
	~CTextOneLineData();

	float line_width;
	float line_height;

	vector<CCharacterPrimitive*> primitives;
};

class CFontRenderer
{
private:
	wchar_t m_textBuffer[TEXT_BUFFER_SIZE];
	wstring text;
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

	float GetOffsetX(int line_index);

	float GetOffsetY();

	void Rebuild();

	void Init(CTrueTypeFont* font, int font_size, float interval_x, Color color, EAlignment alignment, SRect2D rect);

public:
	void OnRender(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix);
	void OnRenderDebug(Matrix4x4& modelMatrix);
	virtual CFontRenderer* SetTextRect(SRect2D rect);
	SRect2D GetTextRect();
	CFontRenderer* SetFont(CTrueTypeFont* font);
	CTrueTypeFont* GetFont();
	CFontRenderer* SetText(const wstring text);
	const wstring& GetText();
	CFontRenderer* SetIntervalX(float x);
	CFontRenderer* SetIntervalY(float y);
	CFontRenderer* SetFontSize(int size);
	CFontRenderer* SetColor(Color color);
	CFontRenderer* SetTextAlignment(EAlignment alignment);
};

#endif