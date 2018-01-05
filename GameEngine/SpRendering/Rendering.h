#ifndef _RENDERING_H_
#define _RENDERING_H_

struct RenderingStartUpParams
{

};

class RenderingObject
{
	
};

class RenderingEngine
{
public:
	virtual void StartUp(RenderingStartUpParams& params) = 0;
	virtual void ShutDown() = 0;
	virtual void Render() = 0;
	virtual void SetScreenSize(int width, int height) = 0;
	virtual void MakeRenderContext() = 0;

private:

};

#endif