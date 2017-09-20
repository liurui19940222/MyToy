#include"ColladaTest.h"
#include"ColladaFile.h"
#include"Maker.h"
#include"RawRenderer.h"
#include"SkyBox.h"
#include"MeshFactory.h"
#include<glm\glm.hpp>

CSkyBox* box;

void CColladaTest::OnStart()
{
	_MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	_MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	_MainCameraGo->SetLocalPosition(Vector3(0, 6.8, 10));

	FontManager->LoadFont(1, "C:/Windows/Fonts/simsun.ttc");

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

	//m_texture = CTexture2D::Create("textures/shake.png");

	m_texture = CRenderTexture::Create(512, 512, true);

	CCamera* camera = _Maker->Instantiate("Camera")->AddComponent<CCamera>();
	camera->LayerMask() = Layer::Default;
	camera->Perspective(54.0f, _SCW / _SCH, 1.0f, 1000.0f);
	camera->UpdateViewMatrix();
	camera->SetRenderTexture((CRenderTexture*)m_texture);
	camera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	camera->gameObject->SetTag("CRTes");
	camera->gameObject->SetLocalPosition(_MainCameraGo->GetLocalPosition());
	camera->gameObject->SetLocalEulerAngles(_MainCameraGo->GetLocalEulerAngles());
	camera->gameObject->LookAt(model->GetLocalPosition());

	//box = CSkyBox::Create("textures/skybox/top.jpg", 
	//	NULL, 
	//	"textures/skybox/front.jpg", 
	//	"textures/skybox/back.jpg", 
	//	"textures/skybox/left.jpg", 
	//	"textures/skybox/right.jpg");

	box = CSkyBox::Create("textures/skybox2/top.tga",
		"textures/skybox2/bottom.tga",
		"textures/skybox2/front.tga",
		"textures/skybox2/back.tga",
		"textures/skybox2/left.tga",
		"textures/skybox2/right.tga");

	_MainCamera->SetSkyBox(box);
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
		float weights[] = { 1.0f, 0.0f };
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