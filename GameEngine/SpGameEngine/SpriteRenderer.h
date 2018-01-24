#ifndef _SPRITE_H_
#define _SPRITE_H_

#include"Component.h"
#include"SpRendering\Texture.h"

using namespace spgameengine;

class ENGINE_API CSpriteRenderer : public CComponent
{
	REFLECT_CLASS(CSpriteRenderer)
private:
	PTexture texture;

public:

	CSpriteRenderer* SetTexture(PTexture texture);

	virtual void OnStart() override;

	virtual void OnUpdate() override;

	virtual void OnDestroy() override;
};

#endif