#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

#include<gl\glew.h>
#include"Texture.h"
#include"RenderBuffer.h"

SMART_CLASS(RenderTexture) class RenderTexture : public Texture, public IRenderBuffer
{
private:
	GLuint m_fboHandle;
	GLuint m_rboHandle;

public:
	void MakeBuffer(int width, int height, bool depthBuffer);

	virtual void BindBuffer() override;

	virtual void ReleaseBuffer() override;

	static PRenderTexture Create(int width, int height, bool depthBuffer);
};

#endif