#ifndef _MESH_QUAD_H_
#define _MESH_QUAD_H_

#include"DynamicFactory.h"
#include"MeshProvider.h"
#include"Object.h"

class CMeshQuad : public Object, public MeshProvider
{
	REFLECT_CLASS(CMeshQuad)
	Vector3 vertices[6];
	Vector3 normals[2];
	Vector2 uvs[6];

public:
	virtual void OnInitialize() override;

	virtual Vector3* GetVertexArray() override;

	virtual STriangle* GetTriangleArray() override;

	virtual Vector3* GetNormalArray() override;

	virtual Vector3* GetTangentArray() override;

	virtual Vector2* GetUVArray() override;

	virtual int GetVertexNum() override;

	virtual int GetTriangleNum() override;

	virtual int GetGLMode() override;

	virtual void ReleaseMesh() override;
};

#endif