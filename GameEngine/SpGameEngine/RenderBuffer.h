#ifndef _RENDER_BUFFER_H_
#define _RENDER_BUFFER_H_

#include"ApiDefine.h"

class ENGINE_API CRenderBuffer
{
public:

	virtual void BindBuffer() = 0;

	virtual void ReleaseBuffer() = 0;
};

#endif