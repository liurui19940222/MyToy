#pragma once

#include<vector>
#include"SpCommon\EngineDefine.h"
#include"FontManager.h"
#include"Sprite.h"
#include"Material.h"

BEGIN_NAMESPACE_ENGINE

#define TEXT_BUFFER_SIZE 2048

SMART_CLASS(CharacterPrimitive) class CharacterPrimitive
{
public:
	int						m_Left;
	int						m_Top;
	int						m_AdvanceX;
	float					m_Width;
	float					m_Height;
	Vector3					m_Position;
	SpritePtr				m_Sprite;

	CharacterPrimitive();
	CharacterPrimitive(int left_padding, int top, int advance_x, int width, int height, SpritePtr sprite);

	inline SRect2D GetRect()
	{
		return SRect2D(m_Position.x, m_Position.y, m_Width * 0.5f, m_Height * 0.5f);
	}
};

SMART_CLASS(TextLineData) class TextLineData
{
public:
	float								m_LineWidth;
	float								m_LineHeight;
	vector<CharacterPrimitivePtr>			m_Primitives;

	TextLineData();
};

class FontMeshGenerator
{
private:
	float								m_Interval_x;
	float								m_Interval_y;
	float								m_TotalHeight;
	float								m_PixelScale;
	int									m_FontSize;
	TrueTypeFontPtr						m_Font;
	SRect2D								m_Rect;
	EAlignment							m_Alignment;
	EAlignmentHorizontal				m_AlignmentH;
	EAlignmentVertical					m_AlignmentV;
	bool								m_bSingleLine;
	bool								m_bNeedRebuild;
	wchar_t								m_TextBuffer[TEXT_BUFFER_SIZE];

protected:
	vector<TextLineDataPtr>				m_LineDatas;
	vector<CharacterPrimitivePtr>			m_Primitives;
	wstring								m_Text;
	vector<TexcoordRange>				m_TexcoordRanges;
	vector<Color>						m_Colors;
	vector<SRect2D>						m_RectList;
	vector<Matrix4x4>					m_ModelMatrices;

	void RebuildShapes();
	void Init(TrueTypeFontPtr font, int font_size, float interval_x, EAlignment alignment, SRect2D rect);
	float GetOffsetX(int line_index);
	float GetOffsetY();

	inline void ClearPrimitive() { m_Primitives.clear(); }
	inline void ClearLineData() { m_LineDatas.clear(); }
	inline float GetPixelScale() { return m_PixelScale; }

	template<typename T>
	void Set(T* a, T* b, bool update)
	{
		if (*a != *b)
		{
			*a = *b;
			m_bNeedRebuild |= update;
		}
	}

public:
	void BuildInstanceData();
	inline wstring GetText() const { return m_Text; }
	inline SRect2D GetTextRect() const { return m_Rect; }
	inline TrueTypeFontPtr GetFont() const { return m_Font; }
	inline vector<TexcoordRange>& texcoordRanges() { return m_TexcoordRanges; }
	inline vector<Color>& colors() { return m_Colors; }
	inline vector<SRect2D>&	rects() { return m_RectList; }
	inline vector<Matrix4x4>& modelMatrices() { return m_ModelMatrices; }
	inline void SetFont(TrueTypeFontPtr font) { Set(&m_Font, &font, true); }
	inline void SetText(const wstring text) { Set(&m_Text, const_cast<wstring*>(&text), true); }
	inline void SetTextRect(SRect2D rect) { Set(&m_Rect, &rect, true); }
	inline void SetIntervalX(float x) { Set(&m_Interval_x, &x, true); }
	inline void SetIntervalY(float y) { Set(&m_Interval_y, &y, true); }
	inline void SetFontSize(int size) { Set(&m_FontSize, &size, true); }
	inline void SetSingleLine(bool isSingle) { Set(&m_bSingleLine, &isSingle, true); }
	inline void SetPixelScale(float pixelScale) { Set(&m_PixelScale, &pixelScale, true); }
	inline void SetTextAlignment(EAlignment alignment)
	{
		this->m_Alignment = alignment;
		this->m_AlignmentH = _GetHorizontal(alignment);
		this->m_AlignmentV = _GetVertical(alignment);
	}
	inline TextLineDataPtr GetLineData(int rowIndex)
	{
		if (rowIndex >= (int)m_LineDatas.size())
			return NULL;
		return m_LineDatas[rowIndex];
	}
	static MaterialPtr GetDefaultMaterial();
};

END_NAMESPACE_ENGINE
