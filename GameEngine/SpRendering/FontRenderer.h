#ifndef _FONT_RENDERER_H_
#define _FONT_RENDERER_H_

#include"SpCommon\Math.h"
#include"FontManager.h"
#include"SpCommon\EngineDefine.h"
#include"SpRendering\Renderer.h"
#include"SpRendering\MeshBuffer.h"
#include"SpRendering\Texture2D.h"
#include"SpRendering\Material.h"

#define TEXT_BUFFER_SIZE 2048

class CCharacterPrimitiveBase
{
public:
	CTexture2D* m_texture;
	Vector3 position;
	int left;
	int top;
	int advance_x;
	int width;
	int height;
	float width_x;
	float height_y;

	CCharacterPrimitiveBase(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels);
	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color) = 0;
};

class CCharacterPrimitiveSmart : public CCharacterPrimitiveBase
{
public:
	CMeshBuffer m_buffer;
	CMaterial* m_material;

	CCharacterPrimitiveSmart(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels);
	~CCharacterPrimitiveSmart();
	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color) override;
};

class CCharacterPrimitiveFixed : public CCharacterPrimitiveBase
{
public:
	CCharacterPrimitiveFixed(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels);
	~CCharacterPrimitiveFixed();
	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color) override;
};

class CTextOneLineData
{
public:
	CTextOneLineData();
	~CTextOneLineData();

	float line_width;
	float line_height;

	vector<CCharacterPrimitiveBase*> primitives;
};

class CFontRenderer
{
private:
	ERenderType m_renderType = ERenderType::Smart;
	wchar_t m_textBuffer[TEXT_BUFFER_SIZE];
	wstring text;
	CTrueTypeFont* font;
	float interval_x;
	float interval_y;
	int font_size;
	bool singleLine;
	Color color;
	SRect2D rect;
	EAlignment alignment;
	EAlignmentHorizontal alignment_h;
	EAlignmentVertical alignment_v;
	vector<CCharacterPrimitiveBase*> primitives;
	vector<CTextOneLineData*> lineDatas;

protected:
	void ClearPrimitive();
	void ClearLineData();

	virtual float GetPixelScale();

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
	CFontRenderer* SetSingleLine(bool isSingle);
	CFontRenderer* SetTextAlignment(EAlignment alignment);
	CFontRenderer* SetRenderType(ERenderType type);
	CTextOneLineData* GetLineData(int rowIndex);
};

#endif