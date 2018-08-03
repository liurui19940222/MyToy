#include"RigidBodyGameObject.h"
#include"..\SpCommon\EngineDefine.h"
#include<assert.h>

PxRigidActor* RigidBodyFactory::CreateStaticRigidbody(PxPhysics& sdk, PxTransform& pose, PxMaterial& material, PxGeometry& geometry)
{
	PxRigidActor* actor = sdk.createRigidStatic(pose);
	actor->createShape(geometry, material);
	return actor;
}

PxRigidActor* RigidBodyFactory::CreateDynamicRigidbody(PxPhysics& sdk, PxTransform& pose, PxMaterial& material, PxGeometry& geometry, float density)
{
	PxRigidDynamic* actor = PxCreateDynamic(sdk, pose, geometry, material, density);
	return actor;
}

RigidBodyGameObject::RigidBodyGameObject(const Vector3& pos, const Quaternion& rot, PxPhysics& physX, PxScene& scene, PxMaterial& mat, PxGeometry& shape, float density, bool isStatic) : GameObject()
{
	m_PxScene = &scene;
	m_PxMaterial = &mat;

	SetPosition(pos);
	SetRotation(rot);
	PxTransform pose(PxVec3(pos.x, pos.y, pos.z), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)) * PxQuat(rot.x, rot.y, rot.z, rot.w));

	if (isStatic)
		m_PxActor = RigidBodyFactory::CreateStaticRigidbody(physX, pose, *m_PxMaterial, shape);
	else
		m_PxActor = RigidBodyFactory::CreateDynamicRigidbody(physX, pose, *m_PxMaterial, shape, density);
	m_PxScene->addActor(*m_PxActor);
}

RigidBodyGameObject::~RigidBodyGameObject()
{
	m_PxScene->removeActor(*m_PxActor);
}

PxShape& RigidBodyGameObject::GetShape(int index) const
{
	if (index >= 16)
		fatalError("rigidbody get shape index is out of range");

	PxShape* buffer[16];
	m_PxActor->getShapes(buffer, 16, 0);
	return *buffer[index];
}
