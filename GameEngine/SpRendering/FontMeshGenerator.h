#pragma once
#if false
#include"SpCommon\EngineDefine.h"
#include"FontManager.h"

BEGIN_NAMESPACE_ENGINE

#define TEXT_BUFFER_SIZE 2048

class CCharacterPrimitiveBase
{
public:
	int						m_Left;
	int						m_Top;
	int						m_AdvanceX;
	float					m_Width;
	float					m_Height;
	Vector3					m_Position;

	CCharacterPrimitiveBase(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels);
	virtual ~CCharacterPrimitiveBase();
	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color) = 0;
};

class CTextOneLineData
{
public:
	float					m_LineWidth;
	float					m_LineHeight;

	CTextOneLineData();
	virtual ~CTextOneLineData();
	vector<CCharacterPrimitiveBase*> primitives;
};

class FontMeshGenerator
{
private:
	float					m_Interval_x;
	float					m_Interval_y;
	float					m_TotalHeight;
	int						m_FontSize;
	bool					m_bSingleLine;
	Vector3					m_EffectVector;
	Color					m_EffectColor;
	Color					m_Color;
	wchar_t					m_TextBuffer[TEXT_BUFFER_SIZE];
	wstring					m_Text;
	CTrueTypeFont*			m_Font;
	SRect2D					m_Rect;
	EAlignment				m_Alignment;
	EAlignmentHorizontal	m_AlignmentH;
	EAlignmentVertical		m_AlignmentV;
	vector<CTextOneLineData*>		 m_LineDatas;
	vector<CCharacterPrimitiveBase*> m_Primitives;

protected:
	void ClearPrimitive();
	void ClearLineData();
	virtual float GetPixelScale();
	float GetOffsetX(int line_index);
	float GetOffsetY();
	void Rebuild();
	void Init(CTrueTypeFont* font, int font_size, float interval_x, Color color, EAlignment alignment, SRect2D rect);
	void RenderAllPrimitives(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 offset, const Color& color);

public:
	virtual FontMeshGenerator* SetTextRect(SRect2D rect);
	SRect2D GetTextRect();
	FontMeshGenerator* SetFont(CTrueTypeFont* font);
	FontMeshGenerator* GetFont();
	FontMeshGenerator* SetText(const wstring text);
	const wstring GetText();
	FontMeshGenerator* SetIntervalX(float x);
	FontMeshGenerator* SetIntervalY(float y);
	FontMeshGenerator* SetFontSize(int size);
	FontMeshGenerator* SetColor(Color color);
	FontMeshGenerator* SetSingleLine(bool isSingle);
	FontMeshGenerator* SetTextAlignment(EAlignment alignment);
	FontMeshGenerator* SetEffectVector(const Vector3& v);
	FontMeshGenerator* SetEffectColor(const Color& color);
	CTextOneLineData* GetLineData(int rowIndex);
};

END_NAMESPACE_ENGINE
#endif