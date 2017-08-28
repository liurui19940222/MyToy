#ifndef _CMODEL_FILE_
#define _CMODEL_FILE_

#include "MeshProvider.h"
#include "Math.h"
#include "SourceFile.h"
#include "BoneAnimation.h"

class CModelFile : public ISourceFile
{
public:
	Model* m_model = NULL;
};

#endif