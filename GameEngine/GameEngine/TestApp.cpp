#include"TestApp.h"
#include"Input.h"

void CTestApp::OnStart()
{
	Engine->SetDrawGrid(false);
	MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	MainCameraGo->SetPosition(Vector3(0, 0, 5));
	MainCameraGo->SetLocalEulerAngles(Vector3(0, 180, 0));

	CMeshQuad* quad = Maker->Instantiate<CMeshQuad>();
	go = Maker->Instantiate("testGo");
	go->AddComponent<CMeshRenderer>()->SetModel(quad);
}

void CTestApp::OnUpdate()
{

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