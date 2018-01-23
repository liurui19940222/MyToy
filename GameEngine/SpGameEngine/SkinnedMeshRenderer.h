#ifndef _SKINNED_MESH_RENDERER_H_
#define _SKINNED_MESH_RENDERER_H_

#include"SpRendering\Texture.h"
#include"Component.h"
#include"DynamicFactory.h"
#include"SpRendering\Shader.h"
#include"SpRendering\Renderer.h"
#include"SpRendering\Material.h"
#include"SpRendering\SkeletonAnimation.h"
#include"SpRendering\MeshBuffer.h"
#include"SpAssetLoader\ColladaLoader.h"

class ENGINE_API CSkinnedMeshRenderer : public CComponent, public IRenderer
{
	REFLECT_CLASS(CSkinnedMeshRenderer)
private:
	PSkeleton m_skeleton;
	PMeshBuffer m_mesh;
	PMaterial m_material;

public:

	CSkinnedMeshRenderer* SetSkinningMesh(PMeshBuffer mesh, PSkeleton skeleton);

	CSkinnedMeshRenderer* SetMaterial(PMaterial material);

	virtual void OnStart() override;
	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix) override;
	virtual void Render(PShader shader) override;
	virtual void RenderDebug(Matrix4x4& modelMatrix) override;
};

#endif