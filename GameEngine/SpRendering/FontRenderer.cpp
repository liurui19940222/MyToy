#include"FontRenderer.h"
#include"SpAssetLoader\ImageLoader.h"
#include"SpRendering\MeshFactory.h"
#include"SpCommon\FastPainter.h"

CCharacterPrimitiveBase::CCharacterPrimitiveBase(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels) :left(left_padding), top(top), advance_x(advance_x)
{
	this->width = width;
	this->height = height;
	this->width_x = width * pixelScale;
	this->height_y = height * pixelScale;
	m_texture = CTexture2D::Create((UCHAR*)pixels, width, height);
	m_texture->SetWrapMode(ETexWrapMode::Clamp)->SetFilterMode(ETexFilterMode::Linear);
}

CCharacterPrimitiveBase::~CCharacterPrimitiveBase()
{
	delete m_texture;
}

CCharacterPrimitiveSmart::CCharacterPrimitiveSmart(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels)
	: CCharacterPrimitiveBase(left_padding, top, advance_x, width, height, pixelScale, pixels)
{
	m_texture->SetEnvMode(ETexEnvMode::Replace);
	m_material = new CMaterial();
	m_material->SetMainTexture(m_texture)->SetState(EPiplelineStateType::DepthTest, false);
	m_material->SetShader(CShader::Get("font"));
	Mesh* mesh = _MeshFactory->CreateRectMesh(width_x, height_y);
	m_buffer.MakeBuffer(*mesh);
	delete mesh;
}

CCharacterPrimitiveSmart::~CCharacterPrimitiveSmart()
{
	m_buffer.ReleaseBuffer();
	delete m_material;
}

void CCharacterPrimitiveSmart::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color)
{
	m_material->SetColor(color);
	m_material->Bind();
	m_material->SetParam("M", modelMatrix * Matrix4x4::Translate(pos) * Matrix4x4::Scale(size));
	m_material->SetParam("V", viewMatrix);
	m_material->SetParam("P", projectionMatrix);
	m_material->SetParam("Color", color);
	m_buffer.BindBuffer();
	glDrawArrays(GL_TRIANGLES, 0, m_buffer.GetVertexNum());
	m_material->Unbind();
}

CCharacterPrimitiveFixed::CCharacterPrimitiveFixed(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels)
	: CCharacterPrimitiveBase(left_padding, top, advance_x, width, height, pixelScale, pixels)
{
	m_texture->SetEnvMode(ETexEnvMode::Modulate);
}

CCharacterPrimitiveFixed::~CCharacterPrimitiveFixed()
{

}

void CCharacterPrimitiveFixed::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	float half_w = width_x * 0.5f;
	float half_h = height_y * 0.5f;
	m_texture->Bind();
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

CTextOneLineData::CTextOneLineData() : line_width(0), line_height(0)
{

}

CTextOneLineData::~CTextOneLineData()
{

}

void CFontRenderer::OnRender(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix)
{
	static Vector3 pos;
	static float halfLineHeight;
	if (!font || lineDatas.size() == 0) return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	static float offset_x;
	static float offset_y;

	offset_y = GetOffsetY();
	for (size_t i = 0; i < lineDatas.size(); ++i)
	{
		halfLineHeight = lineDatas[i]->line_height * 0.5f;
		for (size_t j = 0; j < lineDatas[i]->primitives.size(); ++j)
		{
			offset_x = GetOffsetX(i);
			pos = lineDatas[i]->primitives[j]->position + Vector3(offset_x, offset_y, 0);
			lineDatas[i]->primitives[j]->Render(modelMatrix, viewMatrix, projectionMatrix,
				pos, Vector3{ 1.0f, -1.0f, 1 }, color);
		}
	}

	glDisable(GL_BLEND);
}

void CFontRenderer::OnRenderDebug(Matrix4x4& modelMatrix)
{

}

CFontRenderer* CFontRenderer::SetFont(CTrueTypeFont* font)
{
	if (this->font == font)
		return this;
	this->font = font;
	Rebuild();
	return this;
}

CTrueTypeFont* CFontRenderer::GetFont()
{
	return font;
}

CFontRenderer* CFontRenderer::SetText(const wstring text)
{
	if (text == this->text)
		return this;
	this->text = text;
	Rebuild();
	return this;
}

const wstring& CFontRenderer::GetText()
{
	return text;
}

CFontRenderer* CFontRenderer::SetIntervalX(float x)
{
	this->interval_x = x;
	Rebuild();
	return this;
}

CFontRenderer* CFontRenderer::SetIntervalY(float y)
{
	this->interval_y = y;
	Rebuild();
	return this;
}

CFontRenderer* CFontRenderer::SetFontSize(int size)
{
	this->font_size = size;
	return this;
}

CFontRenderer* CFontRenderer::SetColor(Color color)
{
	if (this->color == color)
		return this;
	this->color = color;
	Rebuild();
	return this;
}

CFontRenderer* CFontRenderer::SetSingleLine(bool isSingle)
{
	if (singleLine != isSingle)
	{
		singleLine = isSingle;
		Rebuild();
	}
	return this;
}

CFontRenderer* CFontRenderer::SetTextAlignment(EAlignment alignment)
{
	this->alignment = alignment;
	this->alignment_h = _GetHorizontal(alignment);
	this->alignment_v = _GetVertical(alignment);
	return this;
}

CFontRenderer* CFontRenderer::SetRenderType(ERenderType type)
{
	m_renderType = type;
	return this;
}

CTextOneLineData* CFontRenderer::GetLineData(int rowIndex)
{
	if (rowIndex >= lineDatas.size())
		return NULL;
	return lineDatas[rowIndex];
}

CFontRenderer* CFontRenderer::SetTextRect(SRect2D rect)
{
	if (rect == this->rect)
		return this;
	this->rect = rect;
	Rebuild();
	return this;
}

SRect2D CFontRenderer::GetTextRect()
{
	return rect;
}

float CFontRenderer::GetPixelScale() { return 0.01f; }

float CFontRenderer::GetOffsetX(int line_index)
{
	float line_width = lineDatas[line_index]->line_width;
	if (alignment_h == EAlignmentHorizontal::LEFT)
	{
		return 0.0f;
	}
	else if (alignment_h == EAlignmentHorizontal::CENTER)
	{
		return (rect.half_size_x * 2 - line_width) * 0.5f;
	}
	else if (alignment_h == EAlignmentHorizontal::RIGHT)
	{
		return rect.half_size_x * 2 - line_width;
	}
	return 0;
}

float CFontRenderer::GetOffsetY()
{
	if (alignment_v == EAlignmentVertical::TOP)
	{
		float firstLineHalfHeight = lineDatas[0]->line_height * 0.5f;
		return  -lineDatas[0]->line_height * 0.5 - firstLineHalfHeight;
	}
	else if (alignment_v == EAlignmentVertical::MIDDLE)
	{
		float firstLineHalfHeight = lineDatas[0]->line_height * 0.5f;
		return -(rect.half_size_y * 2 - m_TotalHeight) * 0.5f - firstLineHalfHeight;
	}
	else if (alignment_v == EAlignmentVertical::BOTTOM)
	{
		float lastLineHalfHeight = lineDatas[lineDatas.size() - 1]->line_height * 0.5f;
		return -(rect.half_size_y * 2 - m_TotalHeight) + lastLineHalfHeight;
	}
	return 0;
}

void CFontRenderer::Rebuild()
{
	static float top, left, adv_x, width, pixelScale, start_x, start_y, prevHeight;
	if (!font) return;
	ClearPrimitive();
	ClearLineData();
	for (int i = 0; i < text.size(); i++)
	{
		CCharacterInfo* chInfo = font->GetCharacter(text[i], font_size);
		SBitmapData bitmap;
		chInfo->GetBitmap(&bitmap, Color::white);
		if (m_renderType == ERenderType::Fixed)
			primitives.push_back(new CCharacterPrimitiveFixed(chInfo->left_padding, chInfo->top, chInfo->advance_x, bitmap.width, bitmap.height, GetPixelScale(), bitmap.buffer));
		else
			primitives.push_back(new CCharacterPrimitiveSmart(chInfo->left_padding, chInfo->top, chInfo->advance_x, bitmap.width, bitmap.height, GetPixelScale(), bitmap.buffer));
		free(bitmap.buffer);
	}

	start_x = -rect.half_size_x;
	start_y = +rect.half_size_y;
	CTextOneLineData* lineData = new CTextOneLineData();
	lineData->line_width = -interval_x;
	lineDatas.push_back(lineData);
	pixelScale = GetPixelScale();

	prevHeight = 0;
	float firstLineHeight = 0;
	m_TotalHeight = 0;
	for (size_t i = 0; i < primitives.size(); ++i)
	{
		if (text[i] == *L"\n" && !singleLine)
		{
			start_x = -rect.half_size_x;
			start_y -= (prevHeight + interval_y);
			m_TotalHeight += prevHeight + interval_y;
			lineData = new CTextOneLineData();
			lineData->line_width = -interval_x;
			lineDatas.push_back(lineData);
			continue;
		}
		top = primitives[i]->top * pixelScale;
		left = primitives[i]->left * pixelScale;
		adv_x = primitives[i]->advance_x * pixelScale;
		width = primitives[i]->width_x;

		if (start_x + left + width >= rect.half_size_x && !singleLine)
		{
			start_x = -rect.half_size_x;
			start_y -= (prevHeight + interval_y);
			m_TotalHeight += prevHeight + interval_y;
			lineData = new CTextOneLineData();
			lineData->line_width = -interval_x;
			lineDatas.push_back(lineData);
		}

		primitives[i]->position = Vector3{ start_x + left + width * 0.5f, start_y + top - primitives[i]->height_y * 0.5f, 0 };

		start_x += adv_x + interval_x;
		lineData->line_width += adv_x + interval_x;
		if (primitives[i]->height_y > lineData->line_height)
		{
			lineData->line_height = primitives[i]->height_y;
			prevHeight = lineData->line_height;
			if (lineDatas.size() == 1)
				firstLineHeight = prevHeight;
		}
		lineData->primitives.push_back(primitives[i]);
	}

	static float offset_y, halfLineHeight, pos_y;
	offset_y = GetOffsetY();
	for (int i = (int)lineDatas.size() - 1; i >= 0; i--)
	{
		halfLineHeight = lineDatas[i]->line_height * 0.5f;
		for (size_t j = 0; j < lineDatas[i]->primitives.size(); ++j)
		{
			pos_y = lineDatas[i]->primitives[j]->position.y + offset_y;
			if (pos_y + halfLineHeight > rect.half_size_y || pos_y - halfLineHeight < -rect.half_size_y)
			{
				CTextOneLineData* lineData = lineDatas[i];
				lineDatas.erase(lineDatas.begin() + i);
				delete lineData;
				break;
			}
		}
	}
}

void CFontRenderer::Init(CTrueTypeFont* font, int font_size, float interval_x, Color color, EAlignment alignment, SRect2D rect)
{
	this->font = font;
	this->interval_x = interval_x;
	this->interval_y = interval_y;
	this->color = color;
	this->rect = rect;
	SetFontSize(font_size);
	SetTextAlignment(alignment);
}

void CFontRenderer::ClearPrimitive()
{
	for (size_t i = 0; i < primitives.size(); ++i)
	{
		delete primitives[i];
	}
	primitives.clear();
}

void CFontRenderer::ClearLineData()
{
	for (size_t i = 0; i < lineDatas.size(); ++i)
	{
		delete lineDatas[i];
	}
	lineDatas.clear();
}
