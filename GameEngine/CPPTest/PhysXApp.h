#pragma once

#include"GLAppBase\GLAppBase.h"
#include<physx_3.2\PxPhysicsAPI.h>
#include<iostream>

#define fatalError(error) std::cout << error << std::endl; assert(0);

using namespace std;
using namespace physx;

class PhysXApp : public GLAppBase
{
protected:
	virtual void OnInitialize() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;

public:
	PhysXApp(const wchar_t* className, const wchar_t* title, int width, int height);

private:
	PxPhysics*				m_PhysX;
	PxFoundation*			m_Foundation;
	PxErrorCallback*		m_ErrorCallback;
	PxAllocatorCallback*	m_Allocator;
	PxCooking*				m_Cooking;
	PxScene*				m_PxScene;
	PxMaterial*				m_PxMaterial;
};

