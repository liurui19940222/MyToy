#ifndef _CCAMERA_
#define _CCAMERA_

#include"Component.h"
#include"SpRendering\RenderCamera.h"
#include"DynamicFactory.h"

using namespace spgameengine;

class ENGINE_API CCamera : public CComponent, public RenderCamera
{
	REFLECT_CLASS(CCamera)
	friend class CEngine;

	void PrepareFixed();
public:

	virtual void OnStart() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate() override;

	virtual void Perspective(float fov, float aspect, float znear, float zfar) override;
	virtual void Ortho(float left, float right, float bottom, float top, float znear, float zfar) override;
	virtual void Ortho(float halfSize, float aspect) override;
	virtual void LookAt(const Vector3& eye, const Vector3& center, const Vector3& up) override;
	virtual void SetDepth(int depth) override;
	CCamera* UpdateViewMatrix();

	void BeginOneFrame();
	void EndTheFrame();
};

#endif