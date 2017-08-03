#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

#include<gl\glew.h>
#include"RenderBuffer.h"

class CFrameBuffer : public CRenderBuffer
{
private:
	GLuint m_fboHandle;

public:
	CFrameBuffer();

	void MakeBuffer(int width, int height, int bpp);

	virtual void BindBuffer() override;

	virtual void ReleaseBuffer() override;
};

#endif