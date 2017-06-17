#include "RelationshipTest.h"

void CRelationshipTest::OnStart()
{
	Engine->SetClearColor(Color::black());
	Engine->SetDrawGrid(false);
	MainCamera->SetPosition(Vector3(0, 0, 3));
	MainCamera->SetEulerAngles(Vector3(0, 180, 0));
	InitLight();
	CMeshCube* cube = Engine->CreateObject<CMeshCube>();
	go = Engine->CreateGameObject();
	go->AddComponent<CMeshRenderer>()->SetModel(cube);
	go->SetPosition(Vector3(0, 0.7f, -5));

	for (int i = 0; i < 4; i++)
	{
		childs.push_back(Engine->CreateGameObject());
		childs[i]->SetParent(go);
		childs[i]->SetLocalScale(Vector3(1.5, 0.1f, 0.5f));
		childs[i]->AddComponent<CMeshRenderer>()->SetModel(cube);
	}

	childs[0]->SetLocalPosition(Vector3(1, 0, 0));
	childs[1]->SetLocalPosition(Vector3(-1, 0, 0));
	childs[2]->SetLocalPosition(Vector3(1, 0, 0));
	childs[3]->SetLocalPosition(Vector3(-1, 0, 0));
	childs[2]->SetLocalEulerAngles(Vector3(0, 90, 0));
	childs[3]->SetLocalEulerAngles(Vector3(0, 90, 0));
}

void CRelationshipTest::OnUpdate()
{
	cameraPos.x = CInput::GetAxis("Horizontal") * CTime::deltaTime * moveSpeed;
	cameraPos.z = CInput::GetAxis("Vertical") * CTime::deltaTime * moveSpeed;

	if (go)
	{
		Vector3 euler = go->GetEulerAngles();
		//euler.y += CInput::GetAxis("MouseX") * CTime::deltaTime;
		//euler.x += CInput::GetAxis("MouseY") * CTime::deltaTime;
		euler.y += CTime::deltaTime * 300;

		go->SetEulerAngles(euler);
	}
	if (CInput::GetKeyDown(DIK_V))
	{
		if (childs.size() > 0)
		{
			Engine->DestroyGameObject(childs[0]);
			childs.erase(childs.begin());
		}
	}

	if (CInput::GetKeyDown(DIK_B))
	{
		if (go)
		{
			Engine->DestroyGameObject(go);
			go = NULL;
		}
	}
}

void CRelationshipTest::OnRender()
{

}

void CRelationshipTest::OnClose()
{

}

void CRelationshipTest::GetApplicationInfo(SApplicationInfo* info)
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

void CRelationshipTest::InitLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

