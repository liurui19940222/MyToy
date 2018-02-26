#pragma once

#include<physx_3.2\PxPhysicsAPI.h>
#include<iostream>
#include"GLAppBase\GLAppBase.h"
#include"GameObject.h"
#include"RigidBodyGameObject.h"

#define fatalError(error) std::cout << error << std::endl; assert(0);

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

public:
	PhysXApp(const wchar_t* className, const wchar_t* title, int width, int height);

private:
	void InitCamera();
	void InitPhysX();
	void InitObjects();

	IRenderingInterface*	m_RI;
	PxPhysics*				m_PhysX;
	PxFoundation*			m_Foundation;
	PxErrorCallback*		m_ErrorCallback;
	PxAllocatorCallback*	m_Allocator;
	PxCooking*				m_Cooking;
	PxScene*				m_PxScene;
	PxMaterial*				m_PxMaterial;
	vector<PGameObject>		m_GameObjects;

	Matrix4x4				m_ViewMatrix;
	Matrix4x4				m_ProjMatrix;
};

