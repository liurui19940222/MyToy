#ifndef _MESH_QUAD_H_
#define _MESH_QUAD_H_

#include"DynamicFactory.h"
#include"MeshProvider.h"
#include"Object.h"

class CMeshQuad : public Object, public MeshProvider
{
	REFLECT_CLASS(CMeshQuad)
	constexpr static int VERTEX_NUM = 6;
	Vector3 vertices[VERTEX_NUM];
	Vector3 normals[VERTEX_NUM];
	Vector2 uvs[VERTEX_NUM];

public:
	virtual void OnInitialize() override;

	virtual Vector3* GetVertexArray() override;

	virtual Vector3* GetNormalArray() override;

	virtual Vector2* GetUVArray() override;

	virtual int GetVertexNum() override;

	virtual int GetTriangleNum() override;
};

#endif