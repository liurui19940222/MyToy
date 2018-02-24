#pragma once

#include"Texture.h"
#include<map>
#include<functional>
#include <ExternInclude\rapidxml\rapidxml.hpp>

BEGIN_NAMESPACE_ENGINE

SMART_CLASS(Sprite) class Sprite
{
public:
	PTexture			m_Texture;
	TexcoordRange		m_Range;
	ushort				m_Width;
	ushort				m_Height;
	static PSprite CreateSprite(PTexture texture, TexcoordRange range, ushort width, ushort height);
};

SMART_CLASS(SpriteSet) class SpriteSet
{
	typedef function<void(string key, string value)> attr_call;
public:
	SpriteSet();
	SpriteSet(const char* tex_filename, const char* xml_filename);
	void LoadAtlas(const char* tex_filename, const char* xml_filename);
	PSprite GetSprite(const string& name) const;

private:
	void ForeachAttribute(rapidxml::xml_node<>* node, attr_call func);

	map<string, PSprite>	m_Sprites;
};

END_NAMESPACE_ENGINE