#include"FontRenderer.h"
#include"BitImage.h"
#include"EditorTool.h"
#include"MeshFactory.h"
#include"Maker.h"

CCharacterPrimitive::CCharacterPrimitive(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels) :left(left_padding), top(top), advance_x(advance_x)
{
	this->width = width;
	this->height = height;
	this->width_x = width * pixelScale;
	this->height_y = height * pixelScale;
	m_material = _Maker->Instantiate<CMaterial>();
	m_texture = CTexture2D::Create((UCHAR*)pixels, width, height);
	m_material->SetMainTexture(m_texture);
	m_material->SetShader(CShader::Get("font"));
	Mesh* mesh = _MeshFactory->CreateRectMesh(width_x, height_y);
	m_buffer.MakeBuffer(*mesh);
	delete(mesh);
}

CCharacterPrimitive::~CCharacterPrimitive()
{
	m_buffer.ReleaseBuffer();
	_Maker->Destroy(m_texture);
	_Maker->Destroy(m_material);
}

void CCharacterPrimitive::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color)
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

CTextOneLineData::CTextOneLineData() : line_width(0), line_height(0)
{

}

CTextOneLineData::~CTextOneLineData()
{

}

void CFontRenderer::Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix)
{
	if (!font || text.size() == 0) return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
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

void CFontRenderer::RenderDebug(Matrix4x4& modelMatrix)
{
	CEditorTool::DrawRect(rect, modelMatrix);
}

CFontRenderer* CFontRenderer::SetFont(CTrueTypeFont* font)
{
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
	return this;
}

CFontRenderer* CFontRenderer::SetColor(Color color)
{
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

CFontRenderer* CFontRenderer::SetTextRect(SRect2D rect)
{
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
		return lineDatas[line_index]->primitives[0]->width_x;
	else if (alignment_h == EAlignmentHorizontal::CENTER)
		return (rect.half_size_x * 2 - line_width) * 0.5f;
	else if (alignment_h == EAlignmentHorizontal::RIGHT)
		return rect.half_size_x * 2 - line_width;
	return 0;
}

float CFontRenderer::GetOffsetY()
{
	float ea = lineDatas.size() * eachLineHeight;
	if (alignment_v == EAlignmentVertical::TOP)
		return  eachLineHeight * 0.5;
	else if (alignment_v == EAlignmentVertical::MIDDLE)
		return -rect.half_size_y + ea * 0.5 + eachLineHeight * 0.5;
	else if (alignment_v == EAlignmentVertical::BOTTOM)
		return -rect.half_size_y * 2 + ea + eachLineHeight * 0.5;
	return 0;
}

void CFontRenderer::Rebuild()
{
	ClearPrimitive();
	ClearLineData();
	for (int i = 0; i < text.size(); i++)
	{
		CCharacterInfo* chInfo = font->GetCharacter(text[i], font_size);
		SBitmapData bitmap;
		chInfo->GetBitmap(&bitmap, color);
		primitives.push_back(new CCharacterPrimitive(chInfo->left_padding, chInfo->top, chInfo->advance_x, bitmap.width, bitmap.height, GetPixelScale(), bitmap.buffer));
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
		primitives[i]->position = Vector3{ start_x, start_y - top, 0 };
		start_x += adv_x + interval_x;
		lineData->line_width += left + adv_x + interval_x;
		if (primitives[i]->height_y > lineData->line_height)
			lineData->line_height = primitives[i]->height_y;
		lineData->primitives.push_back(primitives[i]);
	}
	eachLineHeight = 0;
	for (CTextOneLineData* lineData : lineDatas)
	{
		eachLineHeight -= lineData->line_height;
		for (CCharacterPrimitive* primitive : lineData->primitives)
		{
			primitive->position.y += eachLineHeight;
		}
	}
	eachLineHeight = -eachLineHeight / (float)lineDatas.size() + interval_y;
}

void CFontRenderer::Init(CTrueTypeFont* font, int font_size, float interval_x, float interval_y, Color color, EAlignment alignment, SRect2D rect)
{
	this->font = font;
	this->font_size = font_size;
	this->interval_x = interval_x;
	this->interval_y = interval_y;
	this->color = color;
	this->rect = rect;
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