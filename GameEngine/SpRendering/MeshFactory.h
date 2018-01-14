#ifndef _MESH_FACTORY_H_
#define _MESH_FACTORY_H_

#define _MeshFactory CMeshFactory::GetInstance()

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

class CMeshFactory : public CSingleton<CMeshFactory>
{
	map<EMeshType, Mesh*> m_meshes;
	map<EMeshType, CMeshBuffer*> m_buffers;

	Mesh* CreateCube();
	Mesh* CreateQuad();
	Mesh* CreateMesh(const Vector3* vertices, const Vector2* texcoords, const Vector3* normals, int count);

public:
	Mesh* CreateRectMesh(float width, float height);
	Mesh* CreateMesh(EMeshType type);
	Mesh* SharedMesh(EMeshType type);
	CMeshBuffer* CreateBuffer(EMeshType type);
	CMeshBuffer* SharedBuffer(EMeshType type);
};

#endif