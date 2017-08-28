#ifndef _MESH_RENDERER_H_
#define _MESH_RENDERER_H_

#include"Texture.h"
#include"FileHead.h"
#include"Component.h"
#include"DynamicFactory.h"
#include"Shader.h"
#include"Renderer.h"
#include"Material.h"
#include"MeshBuffer.h"

class CMeshRenderer : public CComponent, public IRenderer
{
	REFLECT_CLASS(CMeshRenderer)
private:
	CMeshBuffer* m_mesh;
	CMaterial* m_material;

public:

	CMeshRenderer* SetModel(CMeshBuffer* mesh);

	CMeshRenderer* SetMaterial(CMaterial* material);

	virtual void OnStart() override;
	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix) override;
	virtual void RenderDebug(Matrix4x4& modelMatrix) override;
};

#endif