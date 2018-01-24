#ifndef _TEXT_RENDERER_H_
#define _TEXT_RENDERER_H_

#include"Component.h"
#include"SpRendering\FontManager.h"
#include"SpRendering\FontRenderer.h"
#include"SpCommon\EngineDefine.h"

using namespace spgameengine;

class ENGINE_API CTextRenderer : public CFontRenderer, public CComponent, public IRenderer
{
	REFLECT_CLASS(CTextRenderer)
private:

public:
	virtual void OnStart() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;

	virtual float GetPixelScale() override;

	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix) override;
	virtual void RenderDebug(Matrix4x4& modelMatrix) override;
};

#endif