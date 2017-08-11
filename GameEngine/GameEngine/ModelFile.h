#ifndef _CMODEL_FILE_
#define _CMODEL_FILE_

#include "MeshProvider.h"
#include "Math.h"
#include "SourceFile.h"

class CModelFile : public ISourceFile, public CMeshProvider
{
protected:
	Vector3* m_vertexArray;
	Vector3* m_normalArray;
	Vector3* m_tangentArray;
	Vector2* m_uvArray;
	STriangle* m_triangleArray;
	int m_vertexNum;
	int m_triangleNum;

public:

	virtual Vector3* GetVertexArray() override;

	virtual STriangle* GetTriangleArray() override;

	virtual Vector3* GetNormalArray() override;

	virtual Vector3* GetTangentArray() override;

	virtual Vector2* GetUVArray() override;

	virtual int GetVertexNum() override;

	virtual int GetTriangleNum() override;

	virtual void ReleaseMesh() override;

	virtual int GetGLMode() override;

	virtual void ReleaseSource() override;
};

#endif