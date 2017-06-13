#ifndef _I_MESH_PROVIDER_H_
#define _I_MESH_PROVIDER_H_

#include"Math.h"

class IMeshProvider
{
public:
	virtual Vector3* GetVertexArray() = 0;

	virtual STriangle* GetTriangleArray() = 0;

	virtual Vector3* GetNormalArray() = 0;

	virtual Vector3* GetTangentArray() = 0;

	virtual Vector2* GetUVArray() = 0;

	virtual int GetVertexNum() = 0;

	virtual int GetTriangleNum() = 0;

	virtual void ReleaseMesh() = 0;
};

#endif