#pragma once

#include<physx_3.2\PxPhysicsAPI.h>
#include"GameObject.h"

using namespace std;
using namespace physx;

enum class ERigidBodyShape
{
	Box,
	Capsule,
	Convex,
	HeightField,
	Plane,
	Sphere,
	Triangle,
};

class RigidBodyFactory
{
public:
	static PxActor* Create(PxPhysics& sdk, PxTransform& pose, PxMaterial& material, ERigidBodyShape type, bool isStatic);
};

class RigidBodyGameObject : public GameObject
{
public:
	RigidBodyGameObject(PxPhysics& physX, PxScene& scene, PxMaterial& mat, ERigidBodyShape shape, bool isStatic);
	virtual ~RigidBodyGameObject();

	void SimulatePhysics(float deltaTime);

protected:
	PxScene*				m_PxScene;
	PxActor*				m_PxActor;
	PxMaterial*				m_PxMaterial;
};

