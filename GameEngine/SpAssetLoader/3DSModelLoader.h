#ifndef _C3DS_FILE_
#define _C3DS_FILE_

#include<lib3ds/file.h>
#include<lib3ds/camera.h>
#include<lib3ds/mesh.h>
#include<lib3ds/node.h>
#include<lib3ds/material.h>
#include<lib3ds/matrix.h>
#include<lib3ds/vector.h>
#include<lib3ds/light.h>
#include"ModelLoader.h"
#include"..\SpCommon\ApiDefine.h"

BEGIN_NAMESPACE_ENGINE

class C3DSModelLoader : public ModelLoader
{
private:
	Lib3dsFile* lib3dsfile;

public:

	virtual PModel LoadFromFile(const char* filename) override;

	virtual void ReleaseSource() override;
};

END_NAMESPACE_ENGINE

#endif
