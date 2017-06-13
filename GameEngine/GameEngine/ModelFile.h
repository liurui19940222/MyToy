#ifndef _CMODEL_FILE_
#define _CMODEL_FILE_

#include "MeshProvider.h"
#include "Math.h"
#include "SourceFile.h"

class CModelFile : public CSourceFile, public IMeshProvider
{
protected:
	Vector3* vertexArray;
	Vector3* normalArray;
	Vector3* tangentArray;
	Vector2* uvArray;
	STriangle* triangleArray;
	int vertexNum;
	int triangleNum;

public:
	CModelFile();
	~CModelFile();

	virtual Vector3* GetVertexArray() override;

	virtual STriangle* GetTriangleArray() override;

	virtual Vector3* GetNormalArray() override;

	virtual Vector3* GetTangentArray() override;

	virtual Vector2* GetUVArray() override;

	virtual int GetVertexNum() override;

	virtual int GetTriangleNum() override;

	virtual void ReleaseMesh() override;

	virtual int GetGLMode() override;

	virtual void Release();
};

#endif