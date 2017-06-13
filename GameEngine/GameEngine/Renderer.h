#ifndef _RENDERER_H_
#define _RENDERER_H_

#include"MeshProvider.h"
#include"Texture.h"
#include"FileHead.h"
#include"Component.h"
#include"DynamicFactory.h"

class CRenderer : public CComponent, public DynamicCreate<CRenderer>
{
private:
	IMeshProvider* model;
	CTexture* texture;

public:
	CRenderer();
	~CRenderer();

	CRenderer* SetModel(IMeshProvider* model);

	CRenderer* SetTexture(CTexture* texture);

	virtual void OnUpdate() override;

	virtual void OnRender() override;

	virtual void OnDestroy() override;
};

#endif