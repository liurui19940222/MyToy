#include "ImageTest.h"

void CImageTest::OnStart()
{
	Engine->SetClearColor(Color::black());
	Engine->SetDrawGrid(false);
	MainCamera->SetPosition(Vector3(0, 7, 10));
	MainCamera->SetEulerAngles(Vector3(30, 180, 0));

	CBitImage* image = Engine->CreateImage("textures/model.png");
	CTexture* texture = CTexture::Create(image);
	CModelFile* model = new C3DSFile();
	model->LoadFromFile("models/model.3DS");
	go = Engine->CreateGameObject();
	go->SetLocalScale(Vector3(0.1f, 0.1f, 0.1f));
	go->SetEulerAngles(Vector3(0, 180, 0));
	CMeshRenderer* renderer = go->AddComponent<CMeshRenderer>();
	renderer->SetModel(model);
	renderer->SetTexture(texture);

	CBitImage* ground_image = Engine->CreateImage("textures/dlg01.bmp");
	CTexture* ground_texture = CTexture::Create(ground_image);
	CGameObject* quadGo = Engine->CreateGameObject();
	quadGo->SetPosition(Vector3(0, -3.5f, 0));
	quadGo->SetLocalScale(Vector3(7, 7, 7));
	quadGo->SetEulerAngles(Vector3(-90, 0, 0));
	CMeshQuad* quadMesh = Engine->CreateObject<CMeshQuad>();
	CMeshRenderer* ground_renderer = quadGo->AddComponent<CMeshRenderer>();
	ground_renderer->SetModel(quadMesh);
	ground_renderer->SetTexture(ground_texture);
}

void CImageTest::OnUpdate()
{
	Vector3 euler = go->GetEulerAngles();
	euler.y += CTime::deltaTime * 50;
	go->SetEulerAngles(euler);
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