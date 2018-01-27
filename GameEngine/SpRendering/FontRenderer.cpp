#include"FontRenderer.h"
#include"SpAssetLoader\ImageLoader.h"
#include"SpRendering\MeshFactory.h"
#include"SpCommon\FastPainter.h"
#include"Texture2D.h"

USING_NAMESPACE_ENGINE

CCharacterPrimitiveBase::CCharacterPrimitiveBase(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels) :m_Left(left_padding), m_Top(top), m_AdvanceX(advance_x)
{
	this->m_Width = width * pixelScale;
	this->m_Height = height * pixelScale;
	m_Texture = Texture2D::Create((UCHAR*)pixels, width, height);
	m_Texture->SetWrapMode(ETexWrapMode::Clamp)->SetFilterMode(ETexFilterMode::Linear);
}

CCharacterPrimitiveBase::~CCharacterPrimitiveBase()
{

}

CCharacterPrimitiveSmart::CCharacterPrimitiveSmart(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels)
	: CCharacterPrimitiveBase(left_padding, top, advance_x, width, height, pixelScale, pixels)
{
	m_Texture->SetEnvMode(ETexEnvMode::Replace);
	m_Material = make_shared<Material>();
	m_Material->SetMainTexture(m_Texture)->SetState(EPiplelineStateType::DepthTest, false);
	m_Material->SetShader(Shader::Get("font"));
	PMesh mesh = _MeshFactory->CreateRectMesh(m_Width, m_Height);
	m_Buffer = make_shared<MeshBufferTexcoord>(mesh);
}

CCharacterPrimitiveSmart::~CCharacterPrimitiveSmart()
{

}

void CCharacterPrimitiveSmart::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color)
{
	m_Material->SetColor(color);
	m_Material->Bind();
	m_Material->SetParam("M", modelMatrix * Matrix4x4::Translate(pos) * Matrix4x4::Scale(size));
	m_Material->SetParam("V", viewMatrix);
	m_Material->SetParam("P", projectionMatrix);
	m_Material->SetParam("Color", color);
	m_Buffer->BindBuffer();
	glDrawArrays(GL_TRIANGLES, 0, m_Buffer->GetVertexNum());
	m_Material->Unbind();
}

CCharacterPrimitiveFixed::CCharacterPrimitiveFixed(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels)
	: CCharacterPrimitiveBase(left_padding, top, advance_x, width, height, pixelScale, pixels)
{
	m_Texture->SetEnvMode(ETexEnvMode::Modulate);
}

CCharacterPrimitiveFixed::~CCharacterPrimitiveFixed()
{

}

void CCharacterPrimitiveFixed::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	float half_w = m_Width * 0.5f;
	float half_h = m_Height * 0.5f;
	m_Texture->Bind();
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glScalef(size.x, size.y, size.z);
	glColor4f(color.r, color.g, color.b, color.a);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(half_w, half_h, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-half_w, half_h, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-half_w, -half_h, 0);
	glTexCoord2f(1, 0);
	glVertex3f(half_w, -half_h, 0);
	glEnd();
	glPopMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

CTextOneLineData::CTextOneLineData() : m_LineWidth(0), m_LineHeight(0)
{

}

CTextOneLineData::~CTextOneLineData()
{

}

void CFontRenderer::RenderAllPrimitives(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 offset, const Color& color)
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

void CFontRenderer::OnRender(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix)
{

	if (!m_Font || m_LineDatas.size() == 0) return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (m_Effect == EFontEffect::Shadow)
		RenderAllPrimitives(modelMatrix, viewMatrix, projectionMatrix, m_EffectVector, m_EffectColor);

	RenderAllPrimitives(modelMatrix, viewMatrix, projectionMatrix, Vector3::zero, m_Color);

	glDisable(GL_BLEND);
}

void CFontRenderer::OnRenderDebug(Matrix4x4& modelMatrix)
{

}

CFontRenderer* CFontRenderer::SetFont(CTrueTypeFont* font)
{
	if (this->m_Font == font)
		return this;
	this->m_Font = font;
	Rebuild();
	return this;
}

CTrueTypeFont* CFontRenderer::GetFont()
{
	return m_Font;
}

CFontRenderer* CFontRenderer::SetText(const wstring text)
{
	if (text == this->m_Text)
		return this;
	this->m_Text = text;
	Rebuild();
	return this;
}

const wstring& CFontRenderer::GetText()
{
	return m_Text;
}

CFontRenderer* CFontRenderer::SetIntervalX(float x)
{
	this->m_Interval_x = x;
	Rebuild();
	return this;
}

CFontRenderer* CFontRenderer::SetIntervalY(float y)
{
	this->m_Interval_y = y;
	Rebuild();
	return this;
}

CFontRenderer* CFontRenderer::SetFontSize(int size)
{
	this->m_FontSize = size;
	return this;
}

CFontRenderer* CFontRenderer::SetColor(Color color)
{
	if (this->m_Color == color)
		return this;
	this->m_Color = color;
	Rebuild();
	return this;
}

CFontRenderer* CFontRenderer::SetSingleLine(bool isSingle)
{
	if (m_bSingleLine != isSingle)
	{
		m_bSingleLine = isSingle;
		Rebuild();
	}
	return this;
}

CFontRenderer* CFontRenderer::SetTextAlignment(EAlignment alignment)
{
	this->m_Alignment = alignment;
	this->m_AlignmentH = _GetHorizontal(alignment);
	this->m_AlignmentV = _GetVertical(alignment);
	return this;
}

CFontRenderer* CFontRenderer::SetRenderType(ERenderType type)
{
	m_RenderType = type;
	return this;
}

CFontRenderer* CFontRenderer::SetEffect(EFontEffect effect)
{
	m_Effect = effect;
	return this;
}

CFontRenderer* CFontRenderer::SetEffectVector(const Vector3& v)
{
	m_EffectVector = v * GetPixelScale();
	return this;
}

CFontRenderer* CFontRenderer::SetEffectColor(const Color& color)
{
	m_EffectColor = color;
	return this;
}

CTextOneLineData* CFontRenderer::GetLineData(int rowIndex)
{
	if (rowIndex >= (int)m_LineDatas.size())
		return NULL;
	return m_LineDatas[rowIndex];
}

CFontRenderer* CFontRenderer::SetTextRect(SRect2D rect)
{
	if (rect == this->m_Rect)
		return this;
	this->m_Rect = rect;
	Rebuild();
	return this;
}

SRect2D CFontRenderer::GetTextRect()
{
	return m_Rect;
}

float CFontRenderer::GetPixelScale() { return 0.01f; }

float CFontRenderer::GetOffsetX(int line_index)
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

float CFontRenderer::GetOffsetY()
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

void CFontRenderer::Rebuild()
{
	static float top, left, adv_x, width, pixelScale, start_x, start_y, prevHeight;
	if (!m_Font) return;
	ClearPrimitive();
	ClearLineData();
	for (int i = 0; i < (int)m_Text.size(); i++)
	{
		CCharacterInfo* chInfo = m_Font->GetCharacter(m_Text[i], m_FontSize);
		SBitmapData bitmap;
		chInfo->GetBitmap(&bitmap, Color::white);
		if (m_RenderType == ERenderType::Fixed)
			m_Primitives.push_back(new CCharacterPrimitiveFixed(chInfo->m_LeftPadding, chInfo->m_Top, chInfo->m_AdvanceX, bitmap.width, bitmap.height, GetPixelScale(), bitmap.buffer));
		else
			m_Primitives.push_back(new CCharacterPrimitiveSmart(chInfo->m_LeftPadding, chInfo->m_Top, chInfo->m_AdvanceX, bitmap.width, bitmap.height, GetPixelScale(), bitmap.buffer));
		free(bitmap.buffer);
	}

	start_x = -m_Rect.halfSize.x;
	start_y = +m_Rect.halfSize.y;
	CTextOneLineData* lineData = new CTextOneLineData();
	lineData->m_LineWidth = -m_Interval_x;
	m_LineDatas.push_back(lineData);
	pixelScale = GetPixelScale();

	prevHeight = 0;
	float firstLineHeight = 0;
	m_TotalHeight = 0;
	for (size_t i = 0; i < m_Primitives.size(); ++i)
	{
		if (m_Text[i] == *L"\n" && !m_bSingleLine)
		{
			start_x = -m_Rect.halfSize.x;
			start_y -= (prevHeight + m_Interval_y);
			m_TotalHeight += prevHeight + m_Interval_y;
			lineData = new CTextOneLineData();
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
			lineData = new CTextOneLineData();
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
				CTextOneLineData* lineData = m_LineDatas[i];
				m_LineDatas.erase(m_LineDatas.begin() + i);
				delete lineData;
				break;
			}
		}
	}
}

void CFontRenderer::Init(CTrueTypeFont* font, int font_size, float interval_x, Color color, EAlignment alignment, SRect2D rect)
{
	this->m_Font = font;
	this->m_Interval_x = interval_x;
	this->m_Interval_y = m_Interval_y;
	this->m_Color = color;
	this->m_Rect = rect;
	SetFontSize(font_size);
	SetTextAlignment(alignment);
}

void CFontRenderer::ClearPrimitive()
{
	for (size_t i = 0; i < m_Primitives.size(); ++i)
	{
		delete m_Primitives[i];
	}
	m_Primitives.clear();
}

void CFontRenderer::ClearLineData()
{
	for (size_t i = 0; i < m_LineDatas.size(); ++i)
	{
		delete m_LineDatas[i];
	}
	m_LineDatas.clear();
}
