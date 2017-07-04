#ifndef _RENDERER_H_
#define _RENDERER_H_

#include"MeshProvider.h"
#include"Texture.h"
#include"FileHead.h"
#include"Component.h"
#include"DynamicFactory.h"

class CMeshRenderer : public CComponent
{
	REFLECT_CLASS(CMeshRenderer)
private:
	IMeshProvider* model;
	CTexture* texture;

public:
	CMeshRenderer();
	~CMeshRenderer();

	CMeshRenderer* SetModel(IMeshProvider* model);

	CMeshRenderer* SetTexture(CTexture* texture);

	virtual void OnUpdate() override;

	virtual void OnRender() override;

	virtual void OnDestroy() override;
};

#endif