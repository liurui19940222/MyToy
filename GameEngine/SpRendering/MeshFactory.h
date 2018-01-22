#ifndef _MESH_FACTORY_H_
#define _MESH_FACTORY_H_

#define _MeshFactory MeshFactory::GetInstance()

#include<map>
#include"SpCommon\Singleton.h"
#include"SkeletonAnimation.h"
#include"MeshBuffer.h"

using namespace std;

enum class EMeshType
{
	Cube,
	Quad,
};

class MeshFactory : public CSingleton<MeshFactory>
{
	map<EMeshType, PMesh> m_meshes;
	map<EMeshType, PMeshBuffer> m_buffers;

	PMesh CreateCube();
	PMesh CreateQuad();
	PMesh CreateMesh(const Vector3* vertices, const Vector2* texcoords, const Vector3* normals, int count);

public:
	PMesh CreateRectMesh(float width, float height);
	PMesh CreateMesh(EMeshType type);
	PMesh SharedMesh(EMeshType type);
	PMeshBuffer CreateBuffer(EMeshType type);
	PMeshBuffer SharedBuffer(EMeshType type);
};

#endif