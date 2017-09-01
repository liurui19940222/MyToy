#ifndef _TEXT_RENDERER_H_
#define _TEXT_RENDERER_H_

#include"Component.h"
#include"FontManager.h"
#include"EngineDefine.h"
#include"FontRenderer.h"


class CTextRenderer : public CFontRenderer, public CComponent
{
	REFLECT_CLASS(CTextRenderer)
private:

public:
	virtual void OnStart() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;

	virtual float GetPixelScale() override;
};

#endif