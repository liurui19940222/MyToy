#ifndef _RENDERER_H_
#define _RENDERER_H_

#include"Math.h"

class IRenderer {

public:
	virtual void Render(Matrix4x4& modelMatrix) = 0;

	virtual void RenderDebug(Matrix4x4& modelMatrix) = 0;
};

#endif