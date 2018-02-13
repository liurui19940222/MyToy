#include"FontMeshGenerator.h"
#include"SpCommon\Debug.h"
#include<assert.h>
#include<algorithm>

USING_NAMESPACE_ENGINE;

CharacterPrimitive::CharacterPrimitive() : m_Left(0), m_Top(0), m_AdvanceX(0), m_Width(0), m_Height(0) { }

CharacterPrimitive::CharacterPrimitive(int left_padding, int top, int advance_x, int width, int height, PSprite sprite) : 
	m_Left(left_padding), m_Top(top), m_AdvanceX(advance_x), m_Width(width), m_Height(height), m_Sprite(sprite) { }

TextLineData::TextLineData() : m_LineWidth(0), m_LineHeight(0) { }

float FontMeshGenerator::GetOffsetX(int line_index)
{
	float line_width = m_LineDatas[line_index]->m_LineWidth;
	if (m_AlignmentH == EAlignmentHorizontal::LEFT)
	{
		return 0.0f;
	}
	else if (m_AlignmentH == EAlignmentHorizontal::CENTER)
	{
		return (m_Rect.halfSize.x * 2 - line_width) * 0.5f;
	}
	else if (m_AlignmentH == EAlignmentHorizontal::RIGHT)
	{
		return m_Rect.halfSize.x * 2 - line_width;
	}
	return 0;
}

float FontMeshGenerator::GetOffsetY()
{
	if (m_AlignmentV == EAlignmentVertical::TOP)
	{
		float firstLineHalfHeight = m_LineDatas[0]->m_LineHeight * 0.5f;
		return  -m_LineDatas[0]->m_LineHeight * 0.5f - firstLineHalfHeight;
	}
	else if (m_AlignmentV == EAlignmentVertical::MIDDLE)
	{
		float firstLineHalfHeight = m_LineDatas[0]->m_LineHeight * 0.5f;
		return -(m_Rect.halfSize.y * 2 - m_TotalHeight) * 0.5f - firstLineHalfHeight;
	}
	else if (m_AlignmentV == EAlignmentVertical::BOTTOM)
	{
		float lastLineHalfHeight = m_LineDatas[m_LineDatas.size() - 1]->m_LineHeight * 0.5f;
		return -(m_Rect.halfSize.y * 2 - m_TotalHeight) + lastLineHalfHeight;
	}
	return 0;
}

void FontMeshGenerator::RebuildShapes()
{
	static float top, left, adv_x, width, pixelScale, start_x, start_y, prevHeight;
	static int i = 0, size = 0;
	if (!m_Font || m_Text.empty()) return;
	ClearLineData();
	ClearPrimitive();
	m_Primitives.assign(m_Text.size(), PCharacterPrimitive());
	for_each(m_Text.begin(), m_Text.end(), [&](wchar_t ch) {
		PCharacterInfo chInfo = m_Font->GetCharacter(ch, m_FontSize);
		PSprite sprite = make_shared<Sprite>();
		sprite->m_Texture = chInfo->m_Atlas->m_Texture;
		sprite->m_Range.m_StartingPoint.x = chInfo->m_Rect.x / (float)chInfo->m_Atlas->width();
		sprite->m_Range.m_StartingPoint.y = chInfo->m_Rect.y / (float)chInfo->m_Atlas->height();
		sprite->m_Range.m_Size.x = chInfo->m_Rect.width / (float)chInfo->m_Atlas->width();
		sprite->m_Range.m_Size.y = chInfo->m_Rect.height / (float)chInfo->m_Atlas->height();
		PCharacterPrimitive smart = make_shared<CharacterPrimitive>(chInfo->m_LeftPadding, chInfo->m_Top,
			chInfo->m_AdvanceX, chInfo->m_Rect.width * GetPixelScale(), chInfo->m_Rect.height * GetPixelScale(), sprite);
		m_Primitives[i++] = smart;
	});

	start_x = -m_Rect.halfSize.x;
	start_y = +m_Rect.halfSize.y;
	PTextLineData lineData = make_shared<TextLineData>();
	lineData->m_LineWidth = -m_Interval_x;
	m_LineDatas.push_back(lineData);
	pixelScale = GetPixelScale();

	prevHeight = 0;
	float firstLineHeight = 0;
	m_TotalHeight = 0;
	size = (int)m_Primitives.size();
	for (i = 0; i < size; ++i)
	{
		// is wchar the '\n'
		if (m_Text[i] == 10 && !m_bSingleLine)
		{
			start_x = -m_Rect.halfSize.x;
			start_y -= (prevHeight + m_Interval_y);
			m_TotalHeight += prevHeight + m_Interval_y;
			lineData = make_shared<TextLineData>();
			lineData->m_LineWidth = -m_Interval_x;
			m_LineDatas.push_back(lineData);
			continue;
		}
		top = m_Primitives[i]->m_Top * pixelScale;
		left = m_Primitives[i]->m_Left * pixelScale;
		adv_x = m_Primitives[i]->m_AdvanceX * pixelScale;
		width = m_Primitives[i]->m_Width;

		if (start_x + left + width >= m_Rect.halfSize.x && !m_bSingleLine)
		{
			start_x = -m_Rect.halfSize.x;
			start_y -= (prevHeight + m_Interval_y);
			m_TotalHeight += prevHeight + m_Interval_y;
			lineData = make_shared<TextLineData>();
			lineData->m_LineWidth = -m_Interval_x;
			m_LineDatas.push_back(lineData);
		}

		m_Primitives[i]->m_Position = Vector3{ start_x + left + width * 0.5f, start_y + top - m_Primitives[i]->m_Height * 0.5f, 0 };

		start_x += adv_x + m_Interval_x;
		lineData->m_LineWidth += adv_x + m_Interval_x;
		if (m_Primitives[i]->m_Height > lineData->m_LineHeight)
		{
			lineData->m_LineHeight = m_Primitives[i]->m_Height;
			prevHeight = lineData->m_LineHeight;
			if (m_LineDatas.size() == 1)
				firstLineHeight = prevHeight;
		}
		lineData->m_Primitives.push_back(m_Primitives[i]);
	}

	static float offset_y, halfLineHeight, pos_y;
	offset_y = GetOffsetY();
	for (i = (int)m_LineDatas.size() - 1; i >= 0; i--)
	{
		halfLineHeight = m_LineDatas[i]->m_LineHeight * 0.5f;
		for (size_t j = 0; j < m_LineDatas[i]->m_Primitives.size(); ++j)
		{
			PCharacterPrimitive smart = m_LineDatas[i]->m_Primitives[j];
			pos_y = m_LineDatas[i]->m_Primitives[j]->m_Position.y + offset_y;
			if (pos_y + halfLineHeight > m_Rect.halfSize.y || pos_y - halfLineHeight < -m_Rect.halfSize.y)
			{
				m_LineDatas.erase(m_LineDatas.begin() + i);
				break;
			}
		}
	}
}

void FontMeshGenerator::BuildInstanceData(Matrix4x4& modelMatrix)
{
	Debug::BeginTiming(L"build textmesh cost of millisecond %d");
	if (!m_bNeedRebuild)
		return;
	RebuildShapes();
	static Matrix4x4 mat;
	static Vector3 pos;
	static float halfLineHeight;
	static float offset_x;
	static float offset_y;

	size_t size = m_Primitives.size();
	m_TexcoordRanges.resize(size);
	m_Colors.resize(size);
	m_RectList.resize(size);
	m_ModelMatrices.resize(size);

	offset_y = GetOffsetY();
	for (size_t i = 0, index = 0; i < m_LineDatas.size(); ++i)
	{
		halfLineHeight = m_LineDatas[i]->m_LineHeight * 0.5f;
		for (size_t j = 0; j < m_LineDatas[i]->m_Primitives.size(); ++j)
		{
			assert(index < size);
			offset_x = GetOffsetX(i);
			pos = m_LineDatas[i]->m_Primitives[j]->m_Position + Vector3(offset_x, offset_y, 0);
			mat.MakeTranslate(pos);
			mat[1][1] *= -1.0f;
			m_TexcoordRanges[index] = m_LineDatas[i]->m_Primitives[j]->m_Sprite->m_Range;
			m_Colors[index] = Color::white;
			m_RectList[index] = m_LineDatas[i]->m_Primitives[j]->GetRect();
			m_ModelMatrices[index] = modelMatrix * mat;
			index++;
		}
	}

	m_bNeedRebuild = false;
	Debug::EndTiming();
}

void FontMeshGenerator::Init(PTrueTypeFont font, int font_size, float interval_x, EAlignment alignment, SRect2D rect)
{
	this->m_Font = font;
	this->m_Interval_x = interval_x;
	this->m_Interval_y = m_Interval_y;
	this->m_Rect = rect;
	SetFontSize(font_size);
	SetTextAlignment(alignment);
}