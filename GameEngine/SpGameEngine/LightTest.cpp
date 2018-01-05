#include"LightTest.h"
#include"ColladaFile.h"
#include"Maker.h"
#include"RawRenderer.h"
#include"SkyBox.h"
#include"MeshFactory.h"
#include"Light.h"
#include<glm\glm.hpp>

void CLightTest::OnStart()
{
	_MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	_MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	_MainCameraGo->SetLocalPosition(Vector3(0, 8, 12));

	model = _Maker->Instantiate(L"model");
	model->SetLocalPosition(Vector3(0, -3.5, 0));
	model->SetLocalScale(Vector3::one * 0.05);
	model->SetLocalEulerAngles(Vector3(0, 45, 0));
	CMaterial* model_mat = _Maker->Instantiate<CMaterial>()->SetShader(CShader::Get("light"));
	collada = _Resources->Load<CColladaFile>("models/scene07.xml");
	m_model = collada->m_model;
	Mesh* mesh = &m_model->m_meshes[0];
	CMeshBuffer* buffer = new CMeshBuffer(*mesh, m_model->m_skeleton.m_weights, m_model->m_skeleton.m_indices);
	CSkinnedMeshRenderer* renderer = model->AddComponent<CSkinnedMeshRenderer>()->SetSkinningMesh(buffer, &m_model->m_skeleton)->SetMaterial(model_mat);
	_MainCameraGo->LookAt(model->GetLocalPosition());

	directionalGo = _Maker->Instantiate(L"light0");
	directionalGo->SetLocalEulerAngles(Vector3(50, 0, 0));
	CLight* light0 = directionalGo->AddComponent<CLight>();
	light0->SetColor(Color::white);
	light0->SetIntensity(0.8);
	light0->SetCastShadow(true);

	pointGo = _Maker->Instantiate(L"light1");
	pointGo->SetLocalPosition(Vector3(0, 0.5, 0));
	pointGo->SetLocalScale(Vector3::one * 0.1);
	//pointGo->AddComponent<CMeshRenderer>()->SetModel(_MeshFactory->SharedBuffer(EMeshType::Cube));
	pointLight = pointGo->AddComponent<CLight>();
	pointLight->SetType(ELightType::Point);
	pointLight->SetColor(Color::blue);
	pointLight->SetIntensity(0.6);
}

void CLightTest::OnUpdate()
{
	float h = CInput::GetAxis("Horizontal") * CTime::deltaTime * 30;
	float v = CInput::GetAxis("Vertical") * CTime::deltaTime * 30;

	Vector3 pos = pointGo->GetLocalPosition();
	pos.x += h;
	pos.y += v;
	pointGo->SetLocalPosition(pos);

	CEditorTool::WatchTarget(*_MainCameraGo, model->GetLocalPosition());

	//sprite->LookAt(_MainCameraGo->GetLocalPosition());
	Vector3 euler = directionalGo->GetLocalEulerAngles();
	euler.y += CTime::deltaTime * 20;
	//directionalGo->SetLocalEulerAngles(euler);
}

void CLightTest::OnRender()
{
	//CEditorTool::DrawVector(directionalGo->GetForward(), directionalGo->GetLocalPosition(), Color::red);
}

void CLightTest::OnClose()
{

}

void CLightTest::GetApplicationInfo(SApplicationInfo* info)
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