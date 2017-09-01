#include"ColladaTest.h"
#include"ColladaFile.h"
#include"Maker.h"
#include<glm\glm.hpp>

void CColladaTest::OnStart()
{
	_MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	_MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	_MainCameraGo->SetLocalPosition(Vector3(0, 6.8, 10));

	model = _Maker->Instantiate("model");

	//model->SetLocalScale(Vector3(0.1f, 0.1f, 0.1f) * 2);
	//model->SetLocalEulerAngles(Vector3(0, 90, 0));
	//CMaterial* model_mat = _Maker->Instantiate<CMaterial>()->SetShader(CShader::Get("skinning"))
	//	->SetMainTexture(CTexture2D::Create("textures/longxia.png"));
	//collada = _Resources->Load<CColladaFile>("models/longxia.xml");

	//model->SetLocalScale(Vector3(0.1f, 0.1f, 0.1f) * 10);
	//model->SetLocalEulerAngles(Vector3(0, 0, 0));
	//CMaterial* model_mat = _Maker->Instantiate<CMaterial>()->SetShader(CShader::Get("skinning"));
	//collada = _Resources->Load<CColladaFile>("models/walk.xml");
	
	model->SetLocalPosition(Vector3(0, 1, 0));
	model->SetLocalScale(Vector3(0.1f, 0.1f, 0.1f) * 1.8);
	model->SetLocalEulerAngles(Vector3(0, -70, 0));
	CMaterial* model_mat = _Maker->Instantiate<CMaterial>()->SetShader(CShader::Get("skinning"))
		->SetMainTexture(CTexture2D::Create("textures/shake.png"));
	collada = _Resources->Load<CColladaFile>("models/shake_skin.xml");
	m_model = collada->m_model;
	Mesh* mesh = &m_model->m_meshes[0];
	CMeshBuffer* buffer = new CMeshBuffer(*mesh, m_model->m_skeleton.m_weights, m_model->m_skeleton.m_indices);
	CSkinnedMeshRenderer* renderer = model->AddComponent<CSkinnedMeshRenderer>()->SetSkinningMesh(buffer, &m_model->m_skeleton)->SetMaterial(model_mat);
	_MainCameraGo->LookAt(model->GetLocalPosition());

	m_clips.push_back(_Resources->LoadAnimation("models/shake_move.xml"));
	m_clips.push_back(_Resources->LoadAnimation("models/shake_hit.xml"));
	m_clips.push_back(_Resources->LoadAnimation("models/shake_death.xml"));
	m_clips[1]->m_isLooping = true;
	m_clips[2]->m_isLooping = false;
}

void CColladaTest::OnUpdate()
{
	float h = CInput::GetAxis("Horizontal") * CTime::deltaTime * 100;
	float v = CInput::GetAxis("Vertical") * CTime::deltaTime * 100;

	Vector3 euler = model->GetLocalEulerAngles();
	euler.y += h;
	model->SetLocalEulerAngles(euler);

	if (m_clips.size() > 0)
	{
		vector<AnimationClip*> v(2);
		v[0] = m_clips[0];
		v[1] = m_clips[1];
		float times[] = { CTime::time, CTime::time };
		float weights[] = { 0.8f, 0.1f };
		vector<JointPose> jointPoses = CSkeletonAnimation::Blend(&v[0], times, weights, 2, m_model->m_skeleton);
		//vector<JointPose> jointPoses = CSkeletonAnimation::Sample(*m_clips[1], m_model->m_skeleton, CTime::time, 1);
		CSkeletonAnimation::CalculateGlobalMatrix(m_model->m_skeleton, jointPoses);
		CSkeletonAnimation::CalculateSkinningMatrix(m_model->m_skeleton);
	}

	CEditorTool::WatchTarget(*_MainCameraGo, model->GetLocalPosition());
}

void CColladaTest::OnRender()
{

}

void CColladaTest::OnClose()
{

}

void CColladaTest::GetApplicationInfo(SApplicationInfo* info)
{
	if (info)
	{
		info->windowWidth = _Config->GetValue<int>("window", "width");
		info->windowHeight = _Config->GetValue<int>("window", "height");
		info->isFullScreen = _Config->GetValue<bool>("window", "full_screen");
		info->windowBits = 32;
		info->appName = L"GameEngineTest";
		info->className = L"MainClass";
	}
}