#ifndef _TRUE_TYPE_FONT_H_
#define _TRUE_TYPE_FONT_H_

#include<map>
#include<vector>
#include<ft2build.h>
#include FT_FREETYPE_H
#include"SpCommon\EngineDefine.h"
#include"SpCommon\Atlas.h"
#include"Texture2D.h"

#define CH_MAP_BITMAP_SIZE_W 256
#define CH_MAP_BITMAP_SIZE_H 256

using namespace std;
BEGIN_NAMESPACE_ENGINE

SMART_STRUCT(CharacterAtlas) struct CharacterAtlas
{
	Atlas*					m_Atlas;
	PTexture				m_Texture;

	inline int width() const { return m_Atlas->GetWidth(); }
	inline int height() const { return m_Atlas->GetHeight(); }
	inline bool Get(Rect2D* rect_in_atlas, uint32* out_pixels, Color color) { return m_Atlas->Get(rect_in_atlas, out_pixels, color); }
	inline bool TryPush(int width, int height, int max_height) { return m_Atlas->TryPush(width, height, max_height); }
	inline bool Push(int width, int height, int max_height, uint8* grey_buffer, RGB rgb, Rect2D* out_rect_in_atlas) 
	{
		bool result = m_Atlas->Push(width, height, max_height, grey_buffer, rgb, out_rect_in_atlas); 
		UpdateTexture();
		return result;
	}
	inline void Release() { m_Atlas->Release(); }
	inline void UpdateTexture() { m_Texture->UpdateData(m_Atlas->GetWidth(), m_Atlas->GetHeight(), GL_RGBA, GL_RGBA, (UCHAR*)m_Atlas->GetPixels(), false); }
};

//单个字符信息
class CCharacterInfo
{
public:
	PCharacterAtlas			m_Atlas;
	int						m_Code;
	Rect2D					m_Rect;
	int						m_LeftPadding;
	int						m_Top;
	int						m_AdvanceX;

	CCharacterInfo(int code);

	bool GetBitmap(SBitmapData* out_bitmap, Color color);
};

//某个字号的所有字体
//可包含多张图集，图集的大小预定义
class CTrueTypeFontSize
{
private:
	int						m_FontSize;
	FT_Library*				m_FtLib;
	FT_Face*				m_FtFace;
	vector<PCharacterAtlas>	m_Atlases;
	map<int, CCharacterInfo*> m_Characters;

	PCharacterAtlas GetEnoughAtlas(int width, int height, int max_height);

public:
	CTrueTypeFontSize(int size, FT_Library* ft_lib, FT_Face* ft_face);

	CCharacterInfo* GetCharacter(int code);

	vector<PCharacterAtlas>* GetAtlases();

	void Release();
};

//一个字体类
//按字号包含该字号的图所有信息
class CTrueTypeFont
{
private:
	int						m_Id;
	const char*				m_Name;
	FT_Library				m_FtLib;
	FT_Face					m_FtFace;
	map<int, CTrueTypeFontSize*> m_SizeMap;

public:
	CTrueTypeFont(int id);

	CTrueTypeFont(int id, const char* file_name);

	bool LoadFromPath(const char* file_name);

	CCharacterInfo* GetCharacter(wchar_t ch, int size);

	vector<PCharacterAtlas>* GetAtlases(int size);

	void Release();
};

END_NAMESPACE_ENGINE

#endif