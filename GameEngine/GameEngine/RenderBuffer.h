#ifndef _RENDER_BUFFER_H_
#define _RENDER_BUFFER_H_

class CRenderBuffer
{
public:

	virtual void BindBuffer() = 0;

	virtual void ReleaseBuffer() = 0;
};

#endif