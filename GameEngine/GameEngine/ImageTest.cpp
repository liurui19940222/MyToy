#include "ImageTest.h"

void CImageTest::OnStart()
{
	Engine->SetDrawGrid(false);
	MainCameraGo->SetPosition(Vector3(0, 7, 10));
	MainCameraGo->SetLocalEulerAngles(Vector3(30, 180, 0));
	InitLight();
	CBitImage* image = Engine->CreateImage("textures/model.png");
	CTexture* texture = CTexture::Create(image);
	texture->SetEnvMode(ETexEnvMode::Modulate);
	CModelFile* model = new C3DSFile();
	model->LoadFromFile("models/model.3DS");
	go = Engine->CreateGameObject();
	go->SetLocalScale(Vector3(0.1f, 0.1f, 0.1f));
	go->SetLocalEulerAngles(Vector3(0, 180, 0));
	CMeshRenderer* renderer = go->AddComponent<CMeshRenderer>();
	renderer->SetModel(model);
	renderer->SetTexture(texture);

	CBitImage* ground_image = Engine->CreateImage("textures/dlg01.bmp");
	CTexture* ground_texture = CTexture::Create(ground_image);
	CGameObject* quadGo = Engine->CreateGameObject();
	quadGo->SetPosition(Vector3(0, -3.5f, 0));
	quadGo->SetLocalScale(Vector3(7, 7, 7));
	quadGo->SetLocalEulerAngles(Vector3(-90, 0, 0));
	CMeshQuad* quadMesh = Engine->CreateObject<CMeshQuad>();
	CMeshRenderer* ground_renderer = quadGo->AddComponent<CMeshRenderer>();
	ground_renderer->SetModel(quadMesh);
	ground_renderer->SetTexture(ground_texture);
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
