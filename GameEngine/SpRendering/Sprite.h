#pragma once

#include"Texture.h"

BEGIN_NAMESPACE_ENGINE

SMART_CLASS(Sprite) class Sprite
{
public:
	PTexture			m_Texture;
	TexcoordRange		m_Range;
};

END_NAMESPACE_ENGINE