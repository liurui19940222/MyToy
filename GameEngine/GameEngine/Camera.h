#ifndef _CCAMERA_
#define _CCAMERA_

#include"Component.h"
#include"RenderCamera.h"
#include"DynamicFactory.h"

class CCamera : public CComponent, public CRenderCamera
{
	REFLECT_CLASS(CCamera)
public:

	virtual void OnStart() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;

	virtual CRenderCamera* Perspective(float fov, float aspect, float znear, float zfar);
	virtual CRenderCamera* Ortho(float left, float right, float bottom, float top, float znear, float zfar);
	virtual CRenderCamera* Ortho(float halfSize, float aspect);
	virtual CRenderCamera* LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

	void BeginOneFrame();
	void EndTheFrame();
};

#endif