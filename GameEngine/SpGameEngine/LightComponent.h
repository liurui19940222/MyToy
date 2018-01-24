#pragma once

#include"Component.h"
#include"SpRendering\Light.h"

using namespace spgameengine;

class LightComponent : public Light, public CComponent
{
public:
	static void RenderShadowMap(LightComponent* light);
	virtual void OnStart() override;
	virtual void OnDestroy() override;
	virtual Vector3 GetLightPosOrDirection() const override;
};

