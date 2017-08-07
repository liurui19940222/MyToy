#include"TestApp.h"
#include"Input.h"
#include"Time.h"
#include"MeshCube.h"

void CTestApp::OnStart()
{
	Engine->SetDrawDebug(true);
	Engine->SetDrawGrid(false);
	MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	MainCameraGo->SetLocalPosition(Vector3(0, 0, 5));
	MainCameraGo->SetLocalEulerAngles(Vector3(0, 180, 0));

	CMeshProvider* quad = Maker->Instantiate<CMeshCube>();
	go1 = Maker->Instantiate("parent");
	go1->AddComponent<CMeshRenderer>()->SetModel(quad);

	go2 = Maker->Instantiate("child");
	go2->AddComponent<CMeshRenderer>()->SetModel(quad);
	go2->SetParent(go1);
	go2->SetLocalPosition(Vector3::right * 1.4f);
	//go2->SetLocalScale(Vector3(0.6f, 0.3f, 0.8f));
	go2->SetLocalScale(Vector3::one * 0.5f);

	go3 = Maker->Instantiate("grandchild");
	go3->AddComponent<CMeshRenderer>()->SetModel(quad);
	go3->SetParent(go2);
	go3->SetLocalPosition(Vector3::right * 1.4f);
	go3->SetLocalScale(Vector3::one * 0.5f);

	CEditorTool::PrintTree();
}

void CTestApp::OnUpdate()
{
	float h = CInput::GetAxis("Horizontal") * CTime::deltaTime;
	float v = CInput::GetAxis("Vertical") * CTime::deltaTime;

	Vector3 euler = go1->GetLocalEulerAngles();
	euler.x += v * 300;
	euler.y += h * 300;
	go1->SetLocalEulerAngles(euler);

	euler = go3->GetLocalEulerAngles();
	if (CInput::GetKey(DIK_NUMPAD8))
	{
		euler.x += CTime::deltaTime * 300;
	}

	if (CInput::GetKey(DIK_NUMPAD2))
	{
		euler.x -= CTime::deltaTime * 300;
	}

	if (CInput::GetKey(DIK_NUMPAD4))
	{
		euler.y -= CTime::deltaTime * 300;
	}

	if (CInput::GetKey(DIK_NUMPAD6))
	{
		euler.y += CTime::deltaTime * 300;
	}
	go3->SetLocalEulerAngles(euler);
}

void CTestApp::OnRender()
{
	
}

void CTestApp::OnClose()
{
	
}

void CTestApp::GetApplicationInfo(SApplicationInfo* info)
{
	if (info)
	{
		info->windowWidth = window_width;
		info->windowHeight = window_height;
		info->windowBits = 32;
		info->isFullScreen = false;
		info->appName = L"GameEngineTest";
		info->className = L"MainClass";
	}
}