#pragma once

#include"Component.h"
#include"SpRendering\Light.h"

class CLightComponent : public CLight, public CComponent
{
public:
	static void RenderShadowMap(CLightComponent* light);
	virtual void OnStart() override;
	virtual void OnDestroy() override;
	virtual Vector3 GetLightPosOrDirection() const override;
};

