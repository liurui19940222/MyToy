#ifndef _SPRITE_H_
#define _SPRITE_H_

#include"Component.h"
#include"Texture.h"

class CSpriteRenderer : public CComponent
{
	REFLECT_CLASS(CSpriteRenderer)
private:
	CTexture* texture;

public:
	CSpriteRenderer();
	~CSpriteRenderer();

	CSpriteRenderer* SetTexture(CTexture* texture);

	virtual void OnUpdate() override;

	virtual void OnRender() override;

	virtual void OnDestroy() override;
};

#endif