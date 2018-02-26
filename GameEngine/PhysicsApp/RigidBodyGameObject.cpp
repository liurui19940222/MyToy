#include "RigidBodyGameObject.h"

PxActor* RigidBodyFactory::Create(PxPhysics& sdk, PxTransform& pose, PxMaterial& material, ERigidBodyShape type, bool isStatic)
{
	PxRigidActor* actor = NULL;
	if (isStatic)
		actor = sdk.createRigidStatic(pose);
	else
		actor = sdk.createRigidDynamic(pose);
	switch (type)
	{
	case ERigidBodyShape::Box:
		actor->createShape(PxBoxGeometry(), material);
		break;
	case ERigidBodyShape::Capsule:
		actor->createShape(PxCapsuleGeometry(), material);
		break;
	case ERigidBodyShape::Convex:
		actor->createShape(PxConvexMeshGeometry(), material);
		break;
	case ERigidBodyShape::HeightField:
		actor->createShape(PxHeightFieldGeometry(), material);
		break;
	case ERigidBodyShape::Plane:
		actor->createShape(PxPlaneGeometry(), material);
		break;
	case ERigidBodyShape::Sphere:
		actor->createShape(PxSphereGeometry(), material);
		break;
	case ERigidBodyShape::Triangle:
		actor->createShape(PxTriangleMeshGeometry(), material);
		break;
	}
	return actor;
}

RigidBodyGameObject::RigidBodyGameObject(PxPhysics& physX, PxScene& scene, PxMaterial& mat, ERigidBodyShape shape, bool isStatic) : GameObject()
{
	m_PxScene = &scene;
	m_PxMaterial = &mat;
	PxReal d = 0.0f;
	PxU32 axis = 1;
	PxTransform pose;

	if (axis == 0)
		pose = PxTransform(PxVec3(d, 0.0f, 0.0f));
	else if (axis == 1)
		pose = PxTransform(PxVec3(0.0f, d, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	else if (axis == 2)
		pose = PxTransform(PxVec3(0.0f, 0.0f, d), PxQuat(-PxHalfPi, PxVec3(0.0f, 1.0f, 0.0f)));

	m_PxActor = RigidBodyFactory::Create(physX, pose, *m_PxMaterial, shape, isStatic);
	m_PxScene->addActor(*m_PxActor);
}

RigidBodyGameObject::~RigidBodyGameObject()
{
	m_PxScene->removeActor(*m_PxActor);
}

void RigidBodyGameObject::SimulatePhysics(float deltaTime)
{
	
}