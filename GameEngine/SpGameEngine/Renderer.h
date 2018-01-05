#ifndef _RENDERER_H_
#define _RENDERER_H_

#include"Math.h"
#include"Shader.h"

enum class ERenderType
{
	Fixed,
	Smart,
};

class ENGINE_API IRenderer {

public:
	virtual void Render(CShader* shader);
	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix) = 0;
	virtual void RenderDebug(Matrix4x4& modelMatrix) = 0;
};

#endif