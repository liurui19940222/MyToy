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

	static float start_x = 0;
	static float start_y = 0;

	start_x = -rect.half_size_x;
	start_y = +rect.half_size_y;

	for (size_t i = 0; i < primitives.size(); ++i)
	{
		float top = primitives[i]->top * 0.01f;
		float left = primitives[i]->left * 0.01f;
		float adv_x = primitives[i]->advance_x * 0.01f;
		start_x += left;
		if (start_x + adv_x + interval_x >= rect.half_size_x)
		{
			start_x = -rect.half_size_x;
			start_y -= interval_y;
		}
		if (start_y - top < -rect.half_size_y)
			break;
		primitives[i]->Render(Vector3{ start_x, start_y - top, 0 }, Vector3{ 1.0f, -1.0f, 1 });
		start_x += adv_x + interval_x;
	}

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(-rect.half_size_x, rect.half_size_y, 0);
	glVertex3f(-rect.half_size_x, -rect.half_size_y, 0);
	glVertex3f(rect.half_size_x, -rect.half_size_y, 0);
	glVertex3f(rect.half_size_x, rect.half_size_y, 0);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();
}

void CTextRenderer::OnDestroy()
{
	ClearPrimitive();
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
}

void CTextRenderer::SetAlignment(EAlignment alignment)
{
	this->alignment = alignment;
}

void CTextRenderer::SetRect(SRect2D rect)
{
	this->rect = rect;
}

void CTextRenderer::Rebuild()
{
	ClearPrimitive();
	for (int i = 0; i < lstrlenW(text); i++)
	{
		CCharacterInfo* chInfo = font->GetCharacter(text[i], font_size);
		SBitmapData bitmap;
		chInfo->GetBitmap(&bitmap);

		primitives.push_back(new CCharacterPrimitive(chInfo->left_padding, chInfo->top, chInfo->advance_x, bitmap.width, bitmap.height, bitmap.buffer));
	}
}

void CTextRenderer::Init(CTrueTypeFont* font, const wchar_t* text, int font_size, float interval_x, float interval_y, Color color, EAlignment alignment, SRect2D rect)
{
	this->font = font;
	this->font_size = font_size;
	this->interval_x = interval_x;
	this->interval_y = interval_y;
	this->color = color;
	this->alignment = alignment;
	this->text = text;
	this->rect = rect;
	Rebuild();
}

void CTextRenderer::ClearPrimitive()
{
	if (primitives.size() > 0)
	{
		for (int i = 0; i < primitives.size(); ++i)
		{
			primitives[i]->Release();
		}
		primitives.clear();
	}
}