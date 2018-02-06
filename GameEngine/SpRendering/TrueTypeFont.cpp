#include "TrueTypeFont.h"
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <freetype/ftbitmap.h>

USING_NAMESPACE_ENGINE

#pragma region CCharacterInfo

CCharacterInfo::CCharacterInfo(int code) : m_Code(code) {}

bool CCharacterInfo::GetBitmap(SBitmapData* out_bitmap, Color color)
{
	if (!out_bitmap)
		return false;

	out_bitmap->width = this->m_Rect.width;
	out_bitmap->height = this->m_Rect.height;
	out_bitmap->buffer = (uint32*)malloc(sizeof(uint32) * m_Rect.width * m_Rect.height);
	memset(out_bitmap->buffer, 0, sizeof(uint32) * m_Rect.width * m_Rect.height);
	m_Atlas->Get(&m_Rect, out_bitmap->buffer, color);

	return true;
}

#pragma endregion

#pragma region CTrueTypeFontSize

CTrueTypeFontSize::CTrueTypeFontSize(int size, FT_Library* ft_lib, FT_Face* ft_face) 
	: m_FontSize(size), m_FtLib(ft_lib), m_FtFace(ft_face) {}

CCharacterInfo* CTrueTypeFontSize::GetCharacter(int code)
{
	CCharacterInfo* chInfo = nullptr;
	if (m_Characters.find(code) == m_Characters.end())
	{
		chInfo = new CCharacterInfo(code);
		m_Characters.insert(make_pair(code, chInfo));

		FT_Set_Char_Size(*m_FtFace, 0, (int)m_FontSize * 64, 0, 128);

		FT_Load_Glyph(*m_FtFace, code, FT_LOAD_DEFAULT);

		if ((*m_FtFace)->glyph->format != FT_GLYPH_FORMAT_BITMAP)
		{
			FT_Render_Glyph((*m_FtFace)->glyph, FT_RENDER_MODE_NORMAL);
		}
		FT_GlyphSlot  slot = (*m_FtFace)->glyph;
		int width = slot->bitmap.width;
		int height = slot->bitmap.rows;
		FT_Pos max_height = (*m_FtFace)->size->metrics.ascender >> 6;
		chInfo->m_LeftPadding = slot->bitmap_left;
		chInfo->m_Top = slot->bitmap_top;
		chInfo->m_AdvanceX = slot->advance.x >> 6;
		PCharacterAtlas atlas = GetEnoughAtlas(width, height, max_height);
		atlas->Push(width, height, max_height + 5, slot->bitmap.buffer, RGB{ 255, 255, 255 }, &(chInfo->m_Rect));
		chInfo->m_Atlas = atlas;
	}
	else
	{
		chInfo = m_Characters[code];
	}

	return chInfo;
}

PCharacterAtlas CTrueTypeFontSize::GetEnoughAtlas(int width, int height, int max_height)
{
	for (vector<PCharacterAtlas>::iterator it = m_Atlases.begin(); it != m_Atlases.end(); ++it)
	{
		if ((*it)->TryPush(width, height, max_height))
		{
			return *it;
		}
	}
	Atlas* atlas = new Atlas(CH_MAP_BITMAP_SIZE_W, CH_MAP_BITMAP_SIZE_H, 2);
	PCharacterAtlas catlas = make_shared<CharacterAtlas>();
	catlas->m_Atlas = atlas;
	catlas->m_Texture = Texture2D::Create(NULL, atlas->GetWidth(), atlas->GetHeight());
	catlas->m_Texture->SetWrapMode(ETexWrapMode::Clamp)->SetFilterMode(ETexFilterMode::Linear);
	m_Atlases.push_back(catlas);
	return catlas;
}

void CTrueTypeFontSize::Release()
{
	m_FtLib = nullptr;
	m_FtFace = nullptr;
	for (vector<PCharacterAtlas>::iterator it; it != m_Atlases.end(); ++it)
	{
		(*it)->Release();
	}
}

vector<PCharacterAtlas>* CTrueTypeFontSize::GetAtlases()
{
	return &m_Atlases;
}


#pragma endregion

#pragma region CCharacterInfo

CTrueTypeFont::CTrueTypeFont(int id) : m_Id(id) { }

CTrueTypeFont::CTrueTypeFont(int id, const char* file_name)
{
	CTrueTypeFont::CTrueTypeFont(id);
	LoadFromPath(file_name);
}

bool CTrueTypeFont::LoadFromPath(const char* file_name)
{
	if (FT_Init_FreeType(&m_FtLib) || FT_New_Face(m_FtLib, file_name, 0, &m_FtFace))
	{
		return false;
	}
	m_Name = m_FtFace->family_name;
	return true;
}

CCharacterInfo* CTrueTypeFont::GetCharacter(wchar_t ch, int size)
{
	if (m_SizeMap.find(size) == m_SizeMap.end())
	{
		CTrueTypeFontSize* fontSize = new CTrueTypeFontSize(size, &m_FtLib, &m_FtFace);
		m_SizeMap.insert(make_pair( size, fontSize));
	}
	long code = FT_Get_Char_Index(m_FtFace, ch);
	return m_SizeMap[size]->GetCharacter(code);
}

vector<PCharacterAtlas>* CTrueTypeFont::GetAtlases(int size)
{
	if (m_SizeMap.find(size) != m_SizeMap.end())
	{
		return m_SizeMap[size]->GetAtlases();
	}
	return nullptr;
}

void CTrueTypeFont::Release()
{
	map<int, CTrueTypeFontSize*>::iterator it = m_SizeMap.begin();
	while (it != m_SizeMap.end())
	{
		(*it).second->Release();
	}
}

#pragma endregion
