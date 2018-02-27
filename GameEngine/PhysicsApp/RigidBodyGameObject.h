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
	static PxRigidActor* CreateStaticRigidbody(PxPhysics& sdk, PxTransform& pose, PxMaterial& material, PxGeometry& geometry);

	static PxRigidActor* CreateDynamicRigidbody(PxPhysics& sdk, PxTransform& pose, PxMaterial& material, PxGeometry& geometry, float density);
};

SMART_CLASS(RigidBodyGameObject) class RigidBodyGameObject : public GameObject
{
public:
	RigidBodyGameObject(const Vector3& pos, const Quaternion& rot, PxPhysics& physX, PxScene& scene, PxMaterial& mat, PxGeometry& shape, float density, bool isStatic);
	virtual ~RigidBodyGameObject();

	PxShape& GetShape(int index) const;

	inline bool isStatic() const { return m_PxActor->isRigidStatic(); }
protected:

	inline PxRigidDynamic* GetDynamic() const { return dynamic_cast<PxRigidDynamic*>(m_PxActor); }
	inline PxRigidStatic* GetStatic() const { return dynamic_cast<PxRigidStatic*>(m_PxActor); }

	PxScene*				m_PxScene;
	PxRigidActor*			m_PxActor;
	PxMaterial*				m_PxMaterial;
};

