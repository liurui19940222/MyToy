#ifndef _RENDERER_H_
#define _RENDERER_H_

#include"MeshProvider.h"
#include"Texture.h"
#include"FileHead.h"
#include"Component.h"
#include"DynamicFactory.h"
#include"Shader.h"

class CMeshRenderer : public CComponent
{
	REFLECT_CLASS(CMeshRenderer)
private:
	MeshProvider* model;
	CTexture* texture;

public:
	CMeshRenderer();
	~CMeshRenderer();

	CMeshRenderer* SetModel(MeshProvider* model);

	CMeshRenderer* SetTexture(CTexture* texture);

	virtual void OnUpdate() override;

	virtual void OnRender() override;

	virtual void OnDestroy() override;
};

#endif