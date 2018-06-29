#pragma once

#include<assimp\Importer.hpp>
#include<assimp\postprocess.h>
#include<assimp\scene.h>
#include"ModelLoader.h"
#include"SpCommon\ApiDefine.h"

BEGIN_NAMESPACE_ENGINE
using namespace Assimp;

class AdvModelLoader : public ModelLoader
{
public:

	virtual void LoadFromFile(const char* filename) override;

	virtual void ReleaseSource() override;
private:
	Importer		m_Importer;

};

END_NAMESPACE_ENGINE
