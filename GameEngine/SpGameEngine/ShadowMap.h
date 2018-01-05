#ifndef _SHADOW_MAP_H_
#define _SHADOW_MAP_H_

#include"Texture.h"

class CShadowMap : public CTexture
{
	REFLECT_CLASS(CShadowMap)
public:
	static CShadowMap* Create(int width, int height);
};

#endif