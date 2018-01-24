#ifndef _TRUE_TYPE_FONT_H_
#define _TRUE_TYPE_FONT_H_

#include<map>
#include<vector>
#include<ft2build.h>
#include FT_FREETYPE_H
#include"SpCommon\EngineDefine.h"
#include"SpCommon\Atlas.h"

using namespace std;
BEGIN_NAMESPACE_ENGINE

//�����ַ���Ϣ
class CCharacterInfo
{
public:
	CAtlas*					m_Atlas;
	int						m_Code;
	Rect2D					m_Rect;
	int						m_LeftPadding;
	int						m_Top;
	int						m_AdvanceX;

	CCharacterInfo(int code);

	bool GetBitmap(SBitmapData* out_bitmap, Color color);
};

//ĳ���ֺŵ���������
//�ɰ�������ͼ����ͼ���Ĵ�СԤ����
class CTrueTypeFontSize
{
private:
	int						m_FontSize;
	FT_Library*				m_FtLib;
	FT_Face*				m_FtFace;
	vector<CAtlas*>			m_Atlases;
	map<int, CCharacterInfo*> m_Characters;

	CAtlas* GetEnoughAtlas(int width, int height, int max_height);

public:
	CTrueTypeFontSize(int size, FT_Library* ft_lib, FT_Face* ft_face);

	CCharacterInfo* GetCharacter(int code);

	vector<CAtlas*>* GetAtlases();

	void Release();
};

//һ��������
//���ֺŰ������ֺŵ�ͼ������Ϣ
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

	vector<CAtlas*>* GetAtlases(int size);

	void Release();
};

END_NAMESPACE_ENGINE

#endif