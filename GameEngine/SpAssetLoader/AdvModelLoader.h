#pragma once

#include<assimp\Importer.hpp>
#include<assimp\postprocess.h>
#include<assimp\scene.h>
#include"..\SpCommon\ApiDefine.h"
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

	virtual ModelPtr LoadFromFile(const char* filename) override;

	AnimationClipPtr LoadAnimationFromFile(const char* filename, Skeleton& skeleton);

	vector<AnimationClipPtr> LoadAnimationsFromFile(const char* filename, Skeleton& skeleton);

	virtual void ReleaseSource() override;
private:
	Importer		m_Importer;

	void ReadMesh(const aiScene* scene, Model* model);
	void ReadSkeleton(const aiScene* scene, Model* model);
	void ReadNode(aiNode* node, int depth, Model* model);
	void ReadAnimation(const aiScene* scene, Model* model, Skeleton& skeleton);

	void AddPositionSample(map<double, JointTransformation>& samples, aiVectorKey& vectorKey, const char* jointName, Skeleton& skeleton);
	void AddRotationSample(map<double, JointTransformation>& samples, aiQuatKey& quatKey, const char* jointName, Skeleton& skeleton);
	void AddScalingSample(map<double, JointTransformation>& samples, aiVectorKey& vectorKey, const char* jointName, Skeleton& skeleton);
	bool GetTransformation(map<double, JointTransformation>& samples, double time, const char* jointName, Transformation** trans, Skeleton& skeleton);

	void CopySampleInfo(JointTransformation& from, AnimationSample& to);
};

END_NAMESPACE_ENGINE
