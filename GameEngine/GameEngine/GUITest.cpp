#include "GUITest.h"

CGameObject* child = NULL;
Vector3 pos;

void GUITest::OnStart()
{
	Engine->SetClearColor(Color::black());
	Engine->SetDrawGrid(false);
	MainCamera->SetPosition(Vector3(0, 0, 10));
	MainCamera->SetEulerAngles(Vector3(0, 180, 0));

	CGameObject* go = Engine->CreateGameObject();
	child = Engine->CreateGameObject();
	child->SetParent(go);
	child->SetLocalPosition(Vector3(2, 0, 0));
	child->AddComponent<CMeshRenderer>()->SetModel(Engine->CreateObject<CMeshCube>());
}

void GUITest::OnUpdate()
{
	float radius = 1;
	pos = child->GetPosition();
	pos.x += CTime::deltaTime;
	child->SetPosition(pos);
}

void GUITest::OnRender()
{
	//Engine->BeginOrtho();
	//
	//CEditorTool::DrawQuad(Vector3(400, 300, 0), 400);

	//Engine->EndOrtho();
}

void GUITest::OnClose()
{

}

void GUITest::GetApplicationInfo(SApplicationInfo* info)
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