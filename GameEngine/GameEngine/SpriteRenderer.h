#ifndef _SPRITE_H_
#define _SPRITE_H_

#include"Component.h"
#include"Texture.h"

class ENGINE_API CSpriteRenderer : public CComponent
{
	REFLECT_CLASS(CSpriteRenderer)
private:
	CTexture* texture;

public:

	CSpriteRenderer* SetTexture(CTexture* texture);

	virtual void OnStart() override;

	virtual void OnUpdate() override;

	virtual void OnDestroy() override;
};

#endif