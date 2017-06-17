#include "GameObject.h"
#include "TextRenderer.h"
//#include "BitImage.h"

CCharacterPrimitive::CCharacterPrimitive(int left_padding, int top, int advance_x, int width, int height, uint32* pixels) :left(left_padding), top(top), advance_x(advance_x), pixels(pixels)
{
	this->width = width;
	this->height = height;
	this->width_x = width * 0.01f;
	this->height_y = height * 0.01f;
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//CBitImage::Create(width, height, 32, (BYTE*)pixels)->Save("F://font.png", FREE_IMAGE_FORMAT::FIF_PNG);
}

void CCharacterPrimitive::Render(Vector3 pos, Vector3 size)
{
	glBindTexture(GL_TEXTURE_2D, texId);
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glScalef(size.x, size.y, size.z);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(0, height_y, 0);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1, 0);
	glVertex3f(width_x, 0, 0);
	glTexCoord2f(1, 1);
	glVertex3f(width_x, height_y, 0);
	glEnd();
	glPopMatrix();
}

void CCharacterPrimitive::Release()
{
	glDeleteTextures(1, &texId);
	if (pixels)
		free(pixels);
}

CTextOneLineData::CTextOneLineData() : line_width(0)
{

}

void CTextOneLineData::Release()
{
	for (int i = 0; i < primitives.size(); ++i)
	{
		primitives[i]->Release();
	}
	primitives.clear();
}

CTextRenderer::CTextRenderer()
{
}


CTextRenderer::~CTextRenderer()
{
}

void CTextRenderer::OnStart()
{

}

void CTextRenderer::OnUpdate()
{

}

void CTextRenderer::OnRender()
{
	if (!font || !text) return;

	glPushMatrix();

	glMultMatrixf((float*)&gameObject->GetModelToWorldMat());

	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	static float offset_x;
	static float offset_y;

	offset_y = GetOffsetY();
	for (size_t i = 0; i < lineDatas.size(); ++i)
	{
		offset_x = GetOffsetX(lineDatas[i]->line_width);
		for (size_t j = 0; j < lineDatas[i]->primitives.size(); ++j)
		{
			lineDatas[i]->primitives[j]->Render(lineDatas[i]->primitives[j]->position + Vector3(offset_x, offset_y, 0), Vector3{ 1.0f, -1.0f, 1 });
		}
	}

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void CTextRenderer::OnDrawDebug()
{
	CEditorTool::DrawRect(rect, gameObject->GetModelToWorldMat());
}

void CTextRenderer::OnDestroy()
{
	ClearPrimitive();
	ClearLineData();
}

void CTextRenderer::SetFont(CTrueTypeFont* font)
{
	this->font = font;
	Rebuild();
}

CTrueTypeFont* CTextRenderer::GetFont()
{
	return font;
}

void CTextRenderer::SetText(const wchar_t* text)
{
	this->text = text;
	Rebuild();
}

const wchar_t* CTextRenderer::GetText()
{
	return text;
}

void CTextRenderer::SetIntervalX(float x)
{
	this->interval_x = x;
}

void CTextRenderer::SetIntervalY(float y)
{
	this->interval_y = y;
}

void CTextRenderer::SetFontSize(int size)
{
	this->font_size = size;
}

void CTextRenderer::SetColor(Color color)
{
	this->color = color;
	Rebuild();
}

void CTextRenderer::SetAlignment(EAlignment alignment)
{
	this->alignment = alignment;
	this->alignment_h = _GetHorizontal(alignment);
	this->alignment_v = _GetVertical(alignment);
}

void CTextRenderer::SetRect(SRect2D rect)
{
	this->rect = rect;
}

float CTextRenderer::GetOffsetX(float line_width)
{
	if (alignment_h == EAlignmentHorizontal::LEFT)
		return 0;
	else if (alignment_h == EAlignmentHorizontal::CENTER)
		return (rect.half_size_x * 2 - line_width) * 0.5f;
	else if (alignment_h == EAlignmentHorizontal::RIGHT)
		return rect.half_size_x * 2 - line_width;
}

float CTextRenderer::GetOffsetY()
{
	if (alignment_v == EAlignmentVertical::TOP)
		return 0;
	else if (alignment_v == EAlignmentVertical::MIDDLE)
		return -(rect.half_size_y * 2 - interval_y * lineDatas.size()) * 0.5f;
	else if (alignment_v == EAlignmentVertical::BOTTOM)
		return -(rect.half_size_y * 2 - interval_y * lineDatas.size());
}

void CTextRenderer::Rebuild()
{
	ClearPrimitive();
	for (int i = 0; i < lstrlenW(text); i++)
	{
		CCharacterInfo* chInfo = font->GetCharacter(text[i], font_size);
		SBitmapData bitmap;
		chInfo->GetBitmap(&bitmap, color);

		primitives.push_back(new CCharacterPrimitive(chInfo->left_padding, chInfo->top, chInfo->advance_x, bitmap.width, bitmap.height, bitmap.buffer));
	}

	ClearLineData();

	static float start_x = 0;
	static float start_y = 0;
	start_x = -rect.half_size_x;
	start_y = +rect.half_size_y;
	CTextOneLineData* lineData = new CTextOneLineData();
	lineDatas.push_back(lineData);
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
		float top = primitives[i]->top * 0.01f;
		float left = primitives[i]->left * 0.01f;
		float adv_x = primitives[i]->advance_x * 0.01f;
		start_x += left;

		if (start_x + adv_x + interval_x >= rect.half_size_x)
		{
			start_x = -rect.half_size_x;
			start_y -= interval_y;
			
			lineData = new CTextOneLineData();
			lineDatas.push_back(lineData);
		}
		if (start_y - top < -rect.half_size_y)
			break;
		primitives[i]->position = Vector3{ start_x, start_y - top, 0 };
		start_x += adv_x + interval_x;
		lineData->line_width += left + adv_x + interval_x;
		lineData->primitives.push_back(primitives[i]);
	}
}

void CTextRenderer::Init(CTrueTypeFont* font, const wchar_t* text, int font_size, float interval_x, float interval_y, Color color, EAlignment alignment, SRect2D rect)
{
	this->font = font;
	this->font_size = font_size;
	this->interval_x = interval_x;
	this->interval_y = interval_y;
	this->color = color;
	this->text = text;
	this->rect = rect;
	SetAlignment(alignment);
	if (lstrlenW(text) > 0)
		Rebuild();
}

void CTextRenderer::ClearPrimitive()
{
	primitives.clear();
}

void CTextRenderer::ClearLineData()
{
	if (lineDatas.size() > 0)
	{
		for (int i = 0; i < lineDatas.size(); ++i)
		{
			lineDatas[i]->Release();
		}
		lineDatas.clear();
	}
}