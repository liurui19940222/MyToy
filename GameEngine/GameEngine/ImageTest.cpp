#include "ImageTest.h"

void CImageTest::OnStart()
{
	Engine->SetDrawGrid(false);
	MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	MainCameraGo->SetLocalPosition(Vector3(0, 7, 10));
	MainCameraGo->SetLocalEulerAngles(Vector3(-30, 180, 0));
	InitLight();
	CBitImage* image = Resources->Load<CBitImage>("textures/model.png");
	CTexture* texture = CTexture2D::Create(image);
	CMaterial* mat_model = Maker->Instantiate<CMaterial>();
	mat_model->SetShader(CShader::Get("texture"))->SetMainTexture(texture);
	texture->SetEnvMode(ETexEnvMode::Modulate);
	CModelFile* model = Resources->Load<C3DSFile>("models/model.3DS");
	go = Maker->Instantiate("model");
	go->SetLocalScale(Vector3(0.1f, 0.1f, 0.1f));
	go->SetLocalEulerAngles(Vector3(0, 180, 0));
	CMeshRenderer* renderer = go->AddComponent<CMeshRenderer>();
	renderer->SetModel(model)->SetMaterial(mat_model);

	CBitImage* ground_image = Resources->Load<CBitImage>("textures/dlg01.bmp");
	CTexture* ground_texture = CTexture2D::Create(ground_image);
	CMaterial* mat_ground = Maker->Instantiate<CMaterial>();
	mat_ground->SetShader(CShader::Get("texture"))->SetMainTexture(ground_texture);
	CGameObject* quadGo = Maker->Instantiate("ground");
	quadGo->SetLocalPosition(Vector3(0, -3.5f, 0));
	quadGo->SetLocalScale(Vector3(7, 7, 7));
	quadGo->SetLocalEulerAngles(Vector3(-90, 0, 0));
	CMeshQuad* quadMesh = Maker->Instantiate<CMeshQuad>();
	CMeshRenderer* ground_renderer = quadGo->AddComponent<CMeshRenderer>();
	ground_renderer->SetModel(quadMesh)->SetMaterial(mat_ground);

	CEditorTool::PrintTree();
}

void CImageTest::OnUpdate()
{
	Vector3 euler = go->GetLocalEulerAngles();
	euler.y += CTime::deltaTime * 50;
	go->SetLocalEulerAngles(euler);
}

void CImageTest::OnRender()
{

}

void CImageTest::OnClose()
{

}

void CImageTest::GetApplicationInfo(SApplicationInfo* info)
{
	if (info)
	{
		info->windowWidth = 800;
		info->windowHeight = 600;
		info->windowBits = 32;
		info->isFullScreen = false;
		info->appName = L"GameEngineTest";
		info->className = L"MainClass";
	}
}

void CImageTest::InitLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}
