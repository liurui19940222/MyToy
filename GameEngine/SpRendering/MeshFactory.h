#ifndef _MESH_FACTORY_H_
#define _MESH_FACTORY_H_

#define _MeshFactory MeshFactory::GetInstance()

#include<map>
#include"SpCommon\Singleton.h"
#include"SkeletonAnimation.h"
#include"MeshBuffer.h"

using namespace std;

BEGIN_NAMESPACE_ENGINE

enum class EMeshType
{
	Cube,
	Quad,
};

class MeshFactory : public Singleton<MeshFactory>
{
	map<EMeshType, MeshPtr> m_Meshes;
	map<EMeshType, MeshBufferPtr> m_buffers;

	MeshPtr CreateCube();
	MeshPtr CreateQuad();
	MeshPtr CreateMesh(const Vector3* vertices, const Vector2* texcoords, const Vector3* normals, const ushort* indices, int vertexCount, int triangleCount);

public:
	MeshPtr CreateRectMesh(float width, float height);
	MeshPtr CreateMesh(EMeshType type);
	MeshPtr SharedMesh(EMeshType type);
	MeshBufferPtr SharedBuffer(EMeshType type);

	template<typename T> shared_ptr<T> CreateBuffer(EMeshType type)
	{
		MeshPtr mesh = CreateMesh(type);
		shared_ptr<T> buffer(new T(CreateMesh(type)));
		return buffer;
	}
};

END_NAMESPACE_ENGINE

#endif