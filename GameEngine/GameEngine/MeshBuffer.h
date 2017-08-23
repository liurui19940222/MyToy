#ifndef _MESH_BUFFER_H_
#define _MESH_BUFFER_H_

#include<gl\glew.h>
#include"RenderBuffer.h"
#include"Math.h"
#include"EngineDefine.h"
#include"BoneAnimation.h"

#define VERTEX_POS			0
#define COLOR_POS			1
#define UV_POS				2
#define NORMAL_POS			3
#define JOINT_INDEX_POS		4
#define JOINT_WEIGHT_POS	5
#define WEIGHT_NUM_POS		6

class CMeshBuffer : public CRenderBuffer
{
private:
	int m_vertexNum;
	GLuint m_vaoHandle;
	GLuint m_vboVertexHandle;
	GLuint m_vboColorHandle;
	GLuint m_vboNormalHandle;
	GLuint m_vboUVHandle;
	GLuint m_vboJointIndexHandle;
	GLuint m_vboJointWeightHandle;
	GLuint m_vboWeigthNumHandle;

public:
	CMeshBuffer();

	void MakeBuffer(const Vector3* vertices, const Color* colors, const Vector3* normals, const Vector2* uvs, int size);

	void MakeVertexBuffer(const Vector3* vertices, int size);

	void MakeColorBuffer(const Color* colors, int size);

	void MakeUVBuffer(const Vector2* uvs, int size);

	void MakeNormalBuffer(const Vector3* normals, int size);

	void MakeJointBuffer(const SkeletonWeight& skeletonWeight);

	virtual void BindBuffer() override;

	virtual void ReleaseBuffer() override;

	int GetVertexNum() const;
};

#endif