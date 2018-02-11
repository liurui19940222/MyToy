#pragma once
#if false
#include"SpCommon\EngineDefine.h"
#include"FontManager.h"

BEGIN_NAMESPACE_ENGINE

#define TEXT_BUFFER_SIZE 2048

class CharacterPrimitiveBase
{
public:
	int						m_Left;
	int						m_Top;
	int						m_AdvanceX;
	float					m_Width;
	float					m_Height;
	Vector3					m_Position;

	CharacterPrimitiveBase(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels);
	virtual ~CharacterPrimitiveBase();
	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color) = 0;
};

class TextOneLineData
{
public:
	float					m_LineWidth;
	float					m_LineHeight;

	TextOneLineData();
	virtual ~TextOneLineData();
	vector<CharacterPrimitiveBase*> primitives;
};

class FontMeshGenerator
{
private:
	float					m_Interval_x;
	float					m_Interval_y;
	float					m_TotalHeight;
	int						m_FontSize;
	bool					m_bSingleLine;
	wchar_t					m_TextBuffer[TEXT_BUFFER_SIZE];
	wstring					m_Text;
	TrueTypeFont*			m_Font;
	SRect2D					m_Rect;
	EAlignment				m_Alignment;
	EAlignmentHorizontal	m_AlignmentH;
	EAlignmentVertical		m_AlignmentV;
	vector<TextOneLineData*>		 m_LineDatas;
	vector<CharacterPrimitiveBase*> m_Primitives;

protected:
	void ClearPrimitive();
	void ClearLineData();
	virtual float GetPixelScale();
	float GetOffsetX(int line_index);
	float GetOffsetY();
	void Rebuild();
	void Init(TrueTypeFont* font, int font_size, float interval_x, Color color, EAlignment alignment, SRect2D rect);
	void RenderAllPrimitives(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 offset, const Color& color);

public:
	virtual FontMeshGenerator* SetTextRect(SRect2D rect);
	SRect2D GetTextRect();
	FontMeshGenerator* SetFont(TrueTypeFont* font);
	FontMeshGenerator* GetFont();
	FontMeshGenerator* SetText(const wstring text);
	const wstring GetText();
	FontMeshGenerator* SetIntervalX(float x);
	FontMeshGenerator* SetIntervalY(float y);
	FontMeshGenerator* SetFontSize(int size);
	FontMeshGenerator* SetSingleLine(bool isSingle);
	FontMeshGenerator* SetTextAlignment(EAlignment alignment);
	TextOneLineData* GetLineData(int rowIndex);
};

END_NAMESPACE_ENGINE
#endif