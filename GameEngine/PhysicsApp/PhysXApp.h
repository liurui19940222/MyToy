#pragma once

#include<physx_3.2\PxPhysicsAPI.h>
#include<iostream>
#include"GLAppBase\GLAppBase.h"
#include"..\SpRendering\RenderCamera.h"
#include"GameObject.h"
#include"RigidBodyGameObject.h"

using namespace std;
using namespace physx;

class PhysXApp : public GLAppBase
{
protected:
	virtual void InitGL(HWND hwnd) override;
	virtual void OnInitialize() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnWindowSizeChanged(int width, int height) override;
	virtual void OnRelease() override;

public:
	PhysXApp(const wchar_t* className, const wchar_t* title, int width, int height);

private:
	void InitCamera();
	void InitPhysX();
	void InitObjects();
	void CreateBox(const Vector3& pos);
	void SimulatePhysics(float deltaTime);
	void HandleInput();

	IRenderingInterface*	m_RI;
	PxPhysics*				m_PhysX;
	PxFoundation*			m_Foundation;
	PxErrorCallback*		m_ErrorCallback;
	PxAllocatorCallback*	m_Allocator;
	PxCooking*				m_Cooking;
	PxScene*				m_PxScene;
	PxMaterial*				m_PxMaterial;
	vector<PGameObject>		m_GameObjects;

	RenderCamera*			m_Camera;
};

