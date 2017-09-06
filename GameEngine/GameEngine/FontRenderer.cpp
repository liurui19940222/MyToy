#include"FontRenderer.h"
#include"BitImage.h"
#include"EditorTool.h"
#include"MeshFactory.h"
#include"Maker.h"

CCharacterPrimitiveBase::CCharacterPrimitiveBase(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels) :left(left_padding), top(top), advance_x(advance_x)
{
	this->width = width;
	this->height = height;
	this->width_x = width * pixelScale;
	this->height_y = height * pixelScale;
	m_texture = CTexture2D::Create((UCHAR*)pixels, width, height);
	m_texture->SetWrapMode(ETexWrapMode::Clamp)->SetFilterMode(ETexFilterMode::Linear);
}

CCharacterPrimitiveSmart::CCharacterPrimitiveSmart(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels)
	: CCharacterPrimitiveBase(left_padding, top, advance_x, width, height, pixelScale, pixels)
{
	m_texture->SetEnvMode(ETexEnvMode::Replace);
	m_material = _Maker->Instantiate<CMaterial>();
	m_material->SetMainTexture(m_texture)->SetState(EPiplelineStateType::DepthTest, false);
	m_material->SetShader(CShader::Get("font"));
	Mesh* mesh = _MeshFactory->CreateRectMesh(width_x, height_y);
	m_buffer.MakeBuffer(*mesh);
	delete(mesh);
}

CCharacterPrimitiveSmart::~CCharacterPrimitiveSmart()
{
	m_buffer.ReleaseBuffer();
	_Maker->Destroy(m_texture);
	_Maker->Destroy(m_material);
}

void CCharacterPrimitiveSmart::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color)
{
	m_material->SetColor(color);
	m_material->Bind();
	m_material->SetParam("M", modelMatrix * Matrix4x4::Translate(pos) * Matrix4x4::Scale(size));
	m_material->SetParam("V", viewMatrix);
	m_material->SetParam("P", projectionMatrix);
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
	_Maker->Destroy(m_texture);
}

void CCharacterPrimitiveFixed::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color)
{
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
}

CTextOneLineData::CTextOneLineData() : line_width(0), line_height(0)
{

}

CTextOneLineData::~CTextOneLineData()
{

}

void CFontRenderer::OnRender(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix)
{
	if (!font || text.size() == 0) return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	static float offset_x;
	static float offset_y;

	offset_y = GetOffsetY();
	for (size_t i = 0; i < lineDatas.size(); ++i)
	{
		offset_x = GetOffsetX(i);
		for (size_t j = 0; j < lineDatas[i]->primitives.size(); ++j)
		{
			lineDatas[i]->primitives[j]->Render(modelMatrix, viewMatrix, projectionMatrix,
				lineDatas[i]->primitives[j]->position + Vector3(offset_x, offset_y, 0), Vector3{ 1.0f, -1.0f, 1 }, color);
		}
	}

	glDisable(GL_BLEND);
}

void CFontRenderer::OnRenderDebug(Matrix4x4& modelMatrix)
{
	CEditorTool::DrawRect(rect, modelMatrix);
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
	return this;
}

CFontRenderer* CFontRenderer::SetIntervalY(float y)
{
	this->interval_y = y;
	return this;
}

CFontRenderer* CFontRenderer::SetFontSize(int size)
{
	this->font_size = size;
	this->interval_y = font_size * GetPixelScale();
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

float CFontRenderer::GetOffsetX(int line_index)
{
	float line_width = lineDatas[line_index]->line_width;
	if (alignment_h == EAlignmentHorizontal::LEFT)
	{
		return lineDatas[line_index]->primitives[0]->width_x * 0.5f;
	}
	else if (alignment_h == EAlignmentHorizontal::CENTER)
	{
		float firstWordHalfWidth = lineDatas[line_index]->primitives[0]->width_x * 0.5f;
		return (rect.half_size_x * 2 - line_width) * 0.5f + firstWordHalfWidth;
	}
	else if (alignment_h == EAlignmentHorizontal::RIGHT)
	{
		float lastWordHalfWidth = lineDatas[line_index]->primitives[lineDatas[line_index]->primitives.size() - 1]->width_x * 0.5f;
		return rect.half_size_x * 2 - line_width + lastWordHalfWidth;
	}
	return 0;
}

float CFontRenderer::GetOffsetY()
{
	if (alignment_v == EAlignmentVertical::TOP)
	{
		return  -interval_y * 0.5;
	}
	else if (alignment_v == EAlignmentVertical::MIDDLE)
	{
		return -(rect.half_size_y * 2 - interval_y  * ((lineDatas.size() - 1))) * 0.5f;
	}
	else if (alignment_v == EAlignmentVertical::BOTTOM)
	{
		float lastLineHalfHeight = lineDatas[lineDatas.size() - 1]->line_height * 0.5f;
		return -(rect.half_size_y * 2 - interval_y * (lineDatas.size())) - lastLineHalfHeight;
	}
	return 0;
}

void CFontRenderer::Rebuild()
{
	if (!font) return;
	ClearPrimitive();
	ClearLineData();
	for (int i = 0; i < text.size(); i++)
	{
		CCharacterInfo* chInfo = font->GetCharacter(text[i], font_size);
		SBitmapData bitmap;
		chInfo->GetBitmap(&bitmap, Color::white);
		if(m_renderType == ERenderType::Fixed)
			primitives.push_back(new CCharacterPrimitiveFixed(chInfo->left_padding, chInfo->top, chInfo->advance_x, bitmap.width, bitmap.height, GetPixelScale(), bitmap.buffer));
		else
			primitives.push_back(new CCharacterPrimitiveSmart(chInfo->left_padding, chInfo->top, chInfo->advance_x, bitmap.width, bitmap.height, GetPixelScale(), bitmap.buffer));
		free(bitmap.buffer);
	}

	static float start_x = 0;
	static float start_y = 0;
	start_x = -rect.half_size_x;
	start_y = +rect.half_size_y;
	CTextOneLineData* lineData = new CTextOneLineData();
	lineDatas.push_back(lineData);
	float pixelScale = GetPixelScale();
	for (size_t i = 0; i < primitives.size(); ++i)
	{
		if (text[i] == *L"\n")
		{
			start_x = -rect.half_size_x;
			start_y -= interval_y;
			lineData = new CTextOneLineData();
			lineDatas.push_back(lineData);
			continue;
		}
		float top = primitives[i]->top * pixelScale;
		float left = primitives[i]->left * pixelScale;
		float adv_x = primitives[i]->advance_x * pixelScale;
		start_x += left;

		if (start_x + adv_x + interval_x >= rect.half_size_x)
		{
			start_x = -rect.half_size_x;
			start_y -= interval_y;

			lineData = new CTextOneLineData();
			lineDatas.push_back(lineData);
		}
		if (start_y - top - interval_y < -rect.half_size_y)
			break;
		primitives[i]->position = Vector3{ start_x, start_y - top * 0.5f, 0 };
		start_x += adv_x + interval_x;
		lineData->line_width += left + adv_x + interval_x;
		if (primitives[i]->height_y > lineData->line_height)
			lineData->line_height = primitives[i]->height_y;
		lineData->primitives.push_back(primitives[i]);
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
		delete(lineDatas[i]);
	}
	lineDatas.clear();
}