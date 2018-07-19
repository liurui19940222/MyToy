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
	struct Transformation
	{
		aiVector3D		position;
		aiQuaternion	rotation;
		aiVector3D		scaling;
	};

	struct JointTransformation
	{
		map<byte, Transformation>	transformations;
	};

public:

	virtual void LoadFromFile(const char* filename) override;

	virtual void ReleaseSource() override;
private:
	Importer		m_Importer;

	void ReadMesh(const aiScene* scene);
	void ReadSkeleton(const aiScene* scene);
	void ReadNode(aiNode* node, int depth);
	void ReadAnimation(const aiScene* scene);

	void AddPositionSample(map<double, JointTransformation>& samples, aiVectorKey& vectorKey, const char* jointName);
	void AddRotationSample(map<double, JointTransformation>& samples, aiQuatKey& quatKey, const char* jointName);
	void AddScalingSample(map<double, JointTransformation>& samples, aiVectorKey& vectorKey, const char* jointName);
	bool GetTransformation(map<double, JointTransformation>& samples, double time, const char* jointName, Transformation** trans);

	void CopySampleInfo(JointTransformation& from, AnimationSample& to);
};

END_NAMESPACE_ENGINE
