#ifndef _MESH_CUBE_H_
#define _MESH_CUBE_H_

#include"DynamicFactory.h"
#include"MeshProvider.h"
#include"Object.h"

class CMeshCube : public Object, public IMeshProvider, public DynamicCreate<CMeshCube>
{
	Vector3 vertices[36];
	Vector3 normals[12];

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