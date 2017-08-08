#ifndef _CCAMERA_
#define _CCAMERA_

#include"Component.h"
#include"RenderCamera.h"
#include"DynamicFactory.h"

class CCamera : public CComponent, public CRenderCamera
{
	REFLECT_CLASS(CCamera)
	friend class CEngine;
public:

	virtual void OnStart() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;

	virtual CRenderCamera* Perspective(float fov, float aspect, float znear, float zfar) override;
	virtual CRenderCamera* Ortho(float left, float right, float bottom, float top, float znear, float zfar) override;
	virtual CRenderCamera* Ortho(float halfSize, float aspect) override;
	virtual CRenderCamera* LookAt(const Vector3& eye, const Vector3& center, const Vector3& up) override;
	virtual CRenderCamera* SetDepth(int depth) override;

	void BeginOneFrame();
	void EndTheFrame();
};

#endif