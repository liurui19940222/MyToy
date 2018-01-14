#ifndef _MESH_RENDERER_H_
#define _MESH_RENDERER_H_

#include"SpRendering\Texture.h"
#include"Component.h"
#include"DynamicFactory.h"
#include"SpRendering\Shader.h"
#include"SpRendering\Renderer.h"
#include"SpRendering\Material.h"
#include"SpRendering\MeshBuffer.h"

class ENGINE_API CMeshRenderer : public CComponent, public IRenderer
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
	virtual void Render(CShader* shader) override;
	virtual void RenderDebug(Matrix4x4& modelMatrix) override;
};

#endif