#include "TrueTypeFont.h"
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <freetype/ftbitmap.h>

#pragma region CCharacterInfo

CCharacterInfo::CCharacterInfo(int code) : code(code) {}

bool CCharacterInfo::GetBitmap(SBitmapData* out_bitmap, Color color)
{
	if (!out_bitmap)
		return false;

	out_bitmap->width = this->rect.width;
	out_bitmap->height = this->rect.height;
	out_bitmap->buffer = (uint32*)malloc(sizeof(uint32) * rect.width * rect.height);
	memset(out_bitmap->buffer, 0, sizeof(uint32) * rect.width * rect.height);
	atlas->Get(&rect, out_bitmap->buffer, color);

	return true;
}

#pragma endregion

#pragma region CTrueTypeFontSize

CTrueTypeFontSize::CTrueTypeFontSize(int size, FT_Library* ft_lib, FT_Face* ft_face) 
	: fontSize(size), ft_lib(ft_lib), ft_face(ft_face) {}

CCharacterInfo* CTrueTypeFontSize::GetCharacter(int code)
{
	CCharacterInfo* chInfo = nullptr;
	if (characters.find(code) == characters.end())
	{
		chInfo = new CCharacterInfo(code);
		characters.insert(make_pair(code, chInfo));

		//FT_Set_Char_Size(*ft_face, fontSize * 48, (int)(fontSize) * 48, 128, 128);
		FT_Set_Pixel_Sizes(*ft_face, fontSize, fontSize);

		FT_Load_Glyph(*ft_face, code, FT_LOAD_DEFAULT);

		if ((*ft_face)->glyph->format != FT_GLYPH_FORMAT_BITMAP)
		{
			FT_Render_Glyph((*ft_face)->glyph, FT_RENDER_MODE_NORMAL);
		}

		int width = (*ft_face)->glyph->bitmap.width;
		int height = (*ft_face)->glyph->bitmap.rows;
		FT_Pos max_height = (*ft_face)->size->metrics.ascender >> 6;
		chInfo->left_padding = -(*ft_face)->glyph->bitmap_left;
		chInfo->top = max_height - (*ft_face)->glyph->bitmap_top;
		//chInfo->top = max_height - ((*ft_face)->glyph->metrics.horiBearingY >> 6);
		chInfo->advance_x = (int)((*ft_face)->glyph->advance.x / 64.0f);
		CAtlas* atlas = GetEnoughAtlas(width, height, max_height);
		atlas->Push(width, height, max_height + 5, (*ft_face)->glyph->bitmap.buffer, RGB{ 255, 255, 255 }, &(chInfo->rect));
		chInfo->atlas = atlas;
	}
	else
	{
		chInfo = characters[code];
	}

	return chInfo;
}

CAtlas* CTrueTypeFontSize::GetEnoughAtlas(int width, int height, int max_height)
{
	for (vector<CAtlas*>::iterator it = atlases.begin(); it != atlases.end(); ++it)
	{
		if ((*it)->TryPush(width, height, max_height))
		{
			return *it;
		}
	}
	CAtlas* atlas = new CAtlas(CH_MAP_BITMAP_SIZE_W, CH_MAP_BITMAP_SIZE_H);
	atlases.push_back(atlas);
	return atlas;
}

void CTrueTypeFontSize::Release()
{
	ft_lib = nullptr;
	ft_face = nullptr;
	for (vector<CAtlas*>::iterator it; it != atlases.end(); ++it)
	{
		(*it)->Release();
	}
}

vector<CAtlas*>* CTrueTypeFontSize::GetAtlases()
{
	return &atlases;
}


#pragma endregion

#pragma region CCharacterInfo

CTrueTypeFont::CTrueTypeFont(int id) : id(id) { }

CTrueTypeFont::CTrueTypeFont(int id, const char* file_name)
{
	CTrueTypeFont::CTrueTypeFont(id);
	LoadFromPath(file_name);
}

bool CTrueTypeFont::LoadFromPath(const char* file_name)
{
	if (FT_Init_FreeType(&ft_lib) || FT_New_Face(ft_lib, file_name, 0, &ft_face))
	{
		return false;
	}
	name = ft_face->family_name;
	return true;
}

CCharacterInfo* CTrueTypeFont::GetCharacter(wchar_t ch, int size)
{
	if (sizeMap.find(size) == sizeMap.end())
	{
		CTrueTypeFontSize* fontSize = new CTrueTypeFontSize(size, &ft_lib, &ft_face);
		sizeMap.insert(make_pair( size, fontSize));
	}
	long code = FT_Get_Char_Index(ft_face, ch);
	return sizeMap[size]->GetCharacter(code);
}

vector<CAtlas*>* CTrueTypeFont::GetAtlases(int size)
{
	if (sizeMap.find(size) != sizeMap.end())
	{
		return sizeMap[size]->GetAtlases();
	}
	return nullptr;
}

void CTrueTypeFont::Release()
{
	map<int, CTrueTypeFontSize*>::iterator it = sizeMap.begin();
	while (it != sizeMap.end())
	{
		(*it).second->Release();
	}
}

#pragma endregion
