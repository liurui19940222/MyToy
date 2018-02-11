#ifndef _RENDERER_H_
#define _RENDERER_H_

#include"Math.h"
#include"Shader.h"

BEGIN_NAMESPACE_ENGINE

class IRenderer {

public:
	virtual void Render(PShader shader) { };
	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix) = 0;
	virtual void RenderDebug(Matrix4x4& modelMatrix) = 0;
};

END_NAMESPACE_ENGINE

#endif