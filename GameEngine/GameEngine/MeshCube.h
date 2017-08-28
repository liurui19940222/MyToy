#ifndef _MESH_CUBE_H_
#define _MESH_CUBE_H_

#include"MeshBuffer.h"
#include"BoneAnimation.h"

class CMeshCube
{
	static constexpr int VERTEX_NUM = 36;
	static Vector3 vertices[VERTEX_NUM];

public:
	static Mesh* Create();
	static CMeshBuffer* CreateBuffer();
};

#endif