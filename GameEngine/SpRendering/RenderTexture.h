#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

#include<gl\glew.h>
#include"Texture.h"
#include"RenderBuffer.h"

BEGIN_NAMESPACE_ENGINE

SMART_CLASS(RenderTexture) class RenderTexture : public Texture, public IRenderBuffer
{
private:
	GLuint m_FboHandle;
	GLuint m_RboHandle;

public:
	RenderTexture();
	virtual ~RenderTexture();
	void MakeBuffer(int width, int height, bool depthBuffer);

	virtual void BindBuffer() override;

	static PRenderTexture Create(int width, int height, bool depthBuffer);
};

END_NAMESPACE_ENGINE

#endif