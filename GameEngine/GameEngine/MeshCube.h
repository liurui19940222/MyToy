#ifndef _MESH_CUBE_H_
#define _MESH_CUBE_H_

#include"DynamicFactory.h"
#include"MeshProvider.h"
#include"Object.h"

class CMeshCube : public Object, public CMeshProvider
{
	REFLECT_CLASS(CMeshCube)
	static constexpr int VERTEX_NUM = 36;
	Vector3 vertices[VERTEX_NUM] = {
		//front
		{-0.5f, 0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f},
		{0.5f, -0.5f, -0.5f},
		{-0.5f, 0.5f, -0.5f},
		{0.5f, -0.5f, -0.5f},
		{0.5f, 0.5f, -0.5f},
		//back
		{0.5f, 0.5f, 0.5f},
		{0.5f, -0.5f, 0.5f},
		{-0.5f, -0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f},
		{-0.5f, -0.5f, 0.5f},
		{-0.5f, 0.5f, 0.5f},
		//left
		{-0.5f, 0.5f, 0.5f},
		{-0.5f, -0.5f, 0.5f},
		{-0.5f, -0.5f, -0.5f},
		{-0.5f, 0.5f, 0.5f},
		{-0.5f, -0.5f, -0.5f},
		{-0.5f, 0.5f, -0.5f},
		//right
		{0.5f, 0.5f, -0.5f},
		{0.5f, -0.5f, -0.5f},
		{0.5f, -0.5f, 0.5f},
		{0.5f, 0.5f, -0.5f},
		{0.5f, -0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f},
		//top
		{-0.5f, 0.5f, 0.5f},
		{-0.5f, 0.5f, -0.5f},
		{0.5f, 0.5f, -0.5f},
		{-0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, -0.5f},
		{0.5f, 0.5f, 0.5f},
		//bottom
		{-0.5f, -0.5f, -0.5f},
		{-0.5f, -0.5f, 0.5f},
		{0.5f, -0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f},
		{0.5f, -0.5f, 0.5f},
		{0.5f, -0.5f, -0.5f},
	};
	Vector3 normals[VERTEX_NUM / 3];
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