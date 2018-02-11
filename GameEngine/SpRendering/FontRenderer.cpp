#include"FontRenderer.h"
#include"SpAssetLoader\ImageLoader.h"
#include"SpRendering\MeshFactory.h"
#include"SpCommon\FastPainter.h"
#include"Texture2D.h"
#include<iostream>

USING_NAMESPACE_ENGINE;

CharacterPrimitiveBase::~CharacterPrimitiveBase() 
{

}

CharacterPrimitiveBase::CharacterPrimitiveBase(int left_padding, int top, int advance_x, int width, int height, float pixelScale) :m_Left(left_padding), m_Top(top), m_AdvanceX(advance_x)
{
	this->m_Width = width * pixelScale;
	this->m_Height = height * pixelScale;
}

CharacterPrimitiveSmart::~CharacterPrimitiveSmart()
{

}

CharacterPrimitiveSmart::CharacterPrimitiveSmart(int left_padding, int top, int advance_x, int width, int height, float pixelScale, PSprite sprite, PMaterial mat)
	: CharacterPrimitiveBase(left_padding, top, advance_x, width, height, pixelScale)
{
	m_Sprite = sprite;
	m_Material = mat;
	PMesh mesh = _MeshFactory->CreateRectMesh(m_Width, m_Height);
	m_Buffer = make_shared<MeshBufferTexcoord>(mesh);
}

void CharacterPrimitiveSmart::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color)
{
	m_Material->SetMainTexture(m_Sprite->m_Texture);
	m_Material->SetColor(color);
	m_Material->Bind();
	static Matrix4x4 mat;
	mat.MakeTranslate(pos);
	mat[0][0] *= size.x;
	mat[1][1] *= size.y;
	mat[2][2] *= size.z;
	m_Material->SetParam("M", modelMatrix * mat);
	m_Material->SetParam("V", viewMatrix);
	m_Material->SetParam("P", projectionMatrix);
	m_Material->SetParam("TexRange", *(Vector4*)&(m_Sprite->m_Range));
	m_Material->SetParam("Color", color);
	m_Buffer->BindBuffer();
	glDrawArrays(GL_TRIANGLES, 0, m_Buffer->GetVertexNum());
	m_Material->Unbind();
}

TextOneLineData::TextOneLineData() : m_LineWidth(0), m_LineHeight(0)
{

}

TextOneLineData::~TextOneLineData() 
{

}

void FontRenderer::RenderAllPrimitives(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 offset, const Color& color)
{
	static Vector3 pos;
	static float halfLineHeight;
	static float offset_x;
	static float offset_y;

	offset_y = GetOffsetY();
	for (size_t i = 0; i < m_LineDatas.size(); ++i)
	{
		halfLineHeight = m_LineDatas[i]->m_LineHeight * 0.5f;
		for (size_t j = 0; j < m_LineDatas[i]->primitives.size(); ++j)
		{
			offset_x = GetOffsetX(i);
			pos = m_LineDatas[i]->primitives[j]->m_Position + Vector3(offset_x, offset_y, 0);
			m_LineDatas[i]->primitives[j]->Render(modelMatrix, viewMatrix, projectionMatrix,
				pos + offset, Vector3{ 1.0f, -1.0f, 1 }, color);
		}
	}
}

void FontRenderer::OnRender(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix)
{
	if (!m_Font || m_LineDatas.empty()) return;

	if (m_Effect == EFontEffect::Shadow)
		RenderAllPrimitives(modelMatrix, viewMatrix, projectionMatrix, m_EffectVector, m_EffectColor);

	RenderAllPrimitives(modelMatrix, viewMatrix, projectionMatrix, Vector3::zero, m_Color);
}

void FontRenderer::OnRenderDebug(Matrix4x4& modelMatrix)
{

}

FontRenderer* FontRenderer::SetFont(PTrueTypeFont font)
{
	if (this->m_Font == font)
		return this;
	this->m_Font = font;
	Rebuild();
	return this;
}

PTrueTypeFont FontRenderer::GetFont()
{
	return m_Font;
}

FontRenderer* FontRenderer::SetText(const wstring text)
{
	if (text == this->m_Text)
		return this;
	this->m_Text = text;
	Rebuild();
	return this;
}

const wstring& FontRenderer::GetText()
{
	return m_Text;
}

FontRenderer* FontRenderer::SetIntervalX(float x)
{
	this->m_Interval_x = x;
	Rebuild();
	return this;
}

FontRenderer* FontRenderer::SetIntervalY(float y)
{
	this->m_Interval_y = y;
	Rebuild();
	return this;
}

FontRenderer* FontRenderer::SetFontSize(int size)
{
	this->m_FontSize = size;
	return this;
}

FontRenderer* FontRenderer::SetColor(Color color)
{
	if (this->m_Color == color)
		return this;
	this->m_Color = color;
	Rebuild();
	return this;
}

FontRenderer* FontRenderer::SetSingleLine(bool isSingle)
{
	if (m_bSingleLine != isSingle)
	{
		m_bSingleLine = isSingle;
		Rebuild();
	}
	return this;
}

FontRenderer* FontRenderer::SetTextAlignment(EAlignment alignment)
{
	this->m_Alignment = alignment;
	this->m_AlignmentH = _GetHorizontal(alignment);
	this->m_AlignmentV = _GetVertical(alignment);
	return this;
}

FontRenderer* FontRenderer::SetEffect(EFontEffect effect)
{
	m_Effect = effect;
	return this;
}

FontRenderer* FontRenderer::SetEffectVector(const Vector3& v)
{
	m_EffectVector = v * GetPixelScale();
	return this;
}

FontRenderer* FontRenderer::SetEffectColor(const Color& color)
{
	m_EffectColor = color;
	return this;
}

PTextOneLineData FontRenderer::GetLineData(int rowIndex)
{
	if (rowIndex >= (int)m_LineDatas.size())
		return NULL;
	return m_LineDatas[rowIndex];
}

FontRenderer* FontRenderer::SetTextRect(SRect2D rect)
{
	if (rect == this->m_Rect)
		return this;
	this->m_Rect = rect;
	Rebuild();
	return this;
}

SRect2D FontRenderer::GetTextRect()
{
	return m_Rect;
}

float FontRenderer::GetPixelScale() { return 0.01f; }

float FontRenderer::GetOffsetX(int line_index)
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

float FontRenderer::GetOffsetY()
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

void FontRenderer::Rebuild()
{
	static float top, left, adv_x, width, pixelScale, start_x, start_y, prevHeight;
	if (!m_Font || m_Text.empty()) return;
	ClearLineData();
	ClearPrimitive();
	for (int i = 0; i < (int)m_Text.size(); i++)
	{
		PCharacterInfo chInfo = m_Font->GetCharacter(m_Text[i], m_FontSize);
		PSprite sprite = make_shared<Sprite>();
		sprite->m_Texture = chInfo->m_Atlas->m_Texture;
		sprite->m_Range.m_StartingPoint.x = chInfo->m_Rect.x / (float)chInfo->m_Atlas->width();
		sprite->m_Range.m_StartingPoint.y = chInfo->m_Rect.y / (float)chInfo->m_Atlas->height();
		sprite->m_Range.m_Size.x = chInfo->m_Rect.width / (float)chInfo->m_Atlas->width();
		sprite->m_Range.m_Size.y = chInfo->m_Rect.height / (float)chInfo->m_Atlas->height();
		m_Primitives.push_back(make_shared<CharacterPrimitiveSmart>(chInfo->m_LeftPadding, chInfo->m_Top,
			chInfo->m_AdvanceX, chInfo->m_Rect.width, chInfo->m_Rect.height, GetPixelScale(), sprite, GetDefaultMaterial()));
	}

	start_x = -m_Rect.halfSize.x;
	start_y = +m_Rect.halfSize.y;
	PTextOneLineData lineData = make_shared<TextOneLineData>();
	lineData->m_LineWidth = -m_Interval_x;
	m_LineDatas.push_back(lineData);
	pixelScale = GetPixelScale();

	prevHeight = 0;
	float firstLineHeight = 0;
	m_TotalHeight = 0;
	size_t size = m_Primitives.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_Text[i] == *L"\n" && !m_bSingleLine)
		{
			start_x = -m_Rect.halfSize.x;
			start_y -= (prevHeight + m_Interval_y);
			m_TotalHeight += prevHeight + m_Interval_y;
			lineData = make_shared<TextOneLineData>();
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
			lineData = make_shared<TextOneLineData>();
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
		lineData->primitives.push_back(m_Primitives[i]);
	}

	static float offset_y, halfLineHeight, pos_y;
	offset_y = GetOffsetY();
	for (int i = (int)m_LineDatas.size() - 1; i >= 0; i--)
	{
		halfLineHeight = m_LineDatas[i]->m_LineHeight * 0.5f;
		for (size_t j = 0; j < m_LineDatas[i]->primitives.size(); ++j)
		{
			pos_y = m_LineDatas[i]->primitives[j]->m_Position.y + offset_y;
			if (pos_y + halfLineHeight > m_Rect.halfSize.y || pos_y - halfLineHeight < -m_Rect.halfSize.y)
			{
				PTextOneLineData lineData = m_LineDatas[i];
				m_LineDatas.erase(m_LineDatas.begin() + i);
				break;
			}
		}
	}
}

void FontRenderer::Init(PTrueTypeFont font, int font_size, float interval_x, Color color, EAlignment alignment, SRect2D rect)
{
	this->m_Font = font;
	this->m_Interval_x = interval_x;
	this->m_Interval_y = m_Interval_y;
	this->m_Color = color;
	this->m_Rect = rect;
	SetFontSize(font_size);
	SetTextAlignment(alignment);
}

void FontRenderer::ClearPrimitive()
{
	if (!m_Primitives.empty())
		m_Primitives.clear();
}

void FontRenderer::ClearLineData()
{
	if (!m_LineDatas.empty())
		m_LineDatas.clear();
}

PMaterial FontRenderer::GetDefaultMaterial()
{
	static PMaterial material;
	if (!material)
	{
		material = make_shared<Material>();
		material->SetState(statetype::DepthTest, false);
		material->SetShader(Shader::Get("font"));
		material->SetState(statetype::Blend, true);
		material->SetBlendFunc(EBlendFactor::SRC_ALPHA, EBlendFactor::ONE_MINUS_SRC_ALPHA);
	}
	return material;
}