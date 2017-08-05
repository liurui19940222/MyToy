#include "RelationshipTest.h"
#include "RenderCamera.h"

void CRelationshipTest::OnStart()
{
	float width = Application->GetWindowWidth();
	float height = Application->GetWindowHeight();
	model = Matrix4x4::Identity();
	CRenderCamera* batch = new CRenderCamera;
	Engine->SetDrawGrid(true);
	MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	MainCameraGo->SetPosition(Vector3(0, 7, 10));
	MainCameraGo->SetLocalEulerAngles(Vector3(-30, 180, 0));
	cameraPos = MainCameraGo->GetPosition();
	CTexture* texture = CTexture2D::Create("textures/dlg01.bmp");
	//InitLight();
	CMeshCube* cube = Maker->Instantiate<CMeshCube>();
	go = Maker->Instantiate("testGo");
	go->AddComponent<CMeshRenderer>()->SetModel(cube)->SetTexture(texture);
	go->SetPosition(Vector3(0, 1.0, 0));
	go->SetLocalScale(Vector3::one * 1);

	for (int i = 0; i < 4; i++)
	{
		string name = "child";
		name += i + '0';
		childs.push_back(Maker->Instantiate(name));
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

	go2 = Maker->Instantiate("testGo2");
	go2->SetParent(childs[0]);
	go2->SetLocalScale(Vector3(2, 1.5, 1));
	go2->SetLocalPosition(Vector3(0, 0, 6));
	go2->AddComponent<CMeshRenderer>()->SetModel(cube);

	Maker->Instantiate("root1");
	Maker->Instantiate("root2");
	Maker->Instantiate("root3");
	CEditorTool::PrintTree();
}

void CRelationshipTest::OnUpdate()
{
	if (!go) return;
	float h = CInput::GetAxis("Horizontal") * CTime::deltaTime * moveSpeed;
	float v = CInput::GetAxis("Vertical") * CTime::deltaTime * moveSpeed;

	
	if (go)
	{
		Vector3 euler = go->GetLocalEulerAngles();
		euler.y += CTime::deltaTime * 30;

		go->SetLocalEulerAngles(euler);
	}
	if (CInput::GetKeyDown(DIK_V))
	{
		if (childs.size() > 0)
		{
			Maker->Destroy(childs[0]);
			childs.erase(childs.begin());
			CEditorTool::PrintTree();
		}
	}

	if (CInput::GetKeyDown(DIK_B))
	{
		if (go)
		{
			Maker->Destroy(go);
			go = NULL;
			CEditorTool::PrintTree();
			return;
		}
	}

	//Vector3 pos = go->GetPosition();
	//pos.x += h * CTime::deltaTime * 10;
	//pos.y += v * CTime::deltaTime * 10;
	//go->SetPosition(pos);
	Vector3 euler = go2->GetLocalEulerAngles();
	euler.y += h * CTime::deltaTime * 10;
	euler.x += v * CTime::deltaTime * 10;
	go2->SetLocalEulerAngles(euler);
}

void CRelationshipTest::OnRender()
{
	if(go)
	CEditorTool::DrawAxis(go);
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

