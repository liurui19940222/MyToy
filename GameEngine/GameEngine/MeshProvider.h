#ifndef _I_MESH_PROVIDER_H_
#define _I_MESH_PROVIDER_H_

#include"Math.h"
#include"MeshBuffer.h"
#include"Camera.h"
struct Color;

class IMeshProvider
{
public:
	virtual Vector3* GetVertexArray() = 0;

	virtual STriangle* GetTriangleArray() = 0;

	virtual Vector3* GetNormalArray() = 0;

	virtual Vector3* GetTangentArray() = 0;

	virtual Vector2* GetUVArray() = 0;

	virtual Color* GetColorArray() = 0;

	virtual int GetVertexNum() = 0;

	virtual int GetTriangleNum() = 0;

	virtual int GetGLMode() = 0;

	virtual void ReleaseMesh() = 0;
};

class MeshProvider : public IMeshProvider
{
protected:
	CMeshBuffer m_buffer;

public:
	virtual STriangle* GetTriangleArray() override;

	virtual Vector3* GetNormalArray() override;

	virtual Vector3* GetTangentArray() override;

	virtual Vector2* GetUVArray() override;

	virtual Color* GetColorArray() override;

	virtual int GetGLMode() override;

	virtual void ReleaseMesh() override;

	CMeshBuffer* GetBuffer();
};

#endif