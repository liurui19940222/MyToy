#ifndef _RENDERING_H_
#define _RENDERING_H_

#include"SkeletonAnimation.h"
#include"Material.h"
#include"MeshBuffer.h"

BEGIN_NAMESPACE_ENGINE

class RenderingStartUpParams
{

};

struct RenderingObject
{
	MeshBuffer* mesh;
	Material* material;
};

class IRenderingInterface
{
public:
	virtual void StartUp(const RenderingStartUpParams* params) = 0;
	virtual void Render(RenderingObject obj) = 0;
	virtual void RenderInstance(RenderingObject obj, uint instanceCount) = 0;
	virtual void MakeRenderContext() = 0;
	virtual void ShutDown() = 0;
};

class GraphicsManager {
private:
	IRenderingInterface* m_RenderingInterface;
};

END_NAMESPACE_ENGINE

#endif