#include "RelationshipTest.h"
#include"SpRendering\RenderCamera.h"
#include"SpRendering\MeshFactory.h"

void CRelationshipTest::OnStart()
{
	model = Matrix4x4::Identity();
	RenderCamera* batch = new RenderCamera;
	_MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	_MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	_MainCameraGo->SetLocalPosition(Vector3(0, 2, 10));
	_MainCameraGo->SetLocalEulerAngles(Vector3(0, -180, 0));
	PTexture texture = Texture2D::Create("textures/dlg01.bmp");
	PMaterial material(new Material());
	material->SetShader(Shader::Get("texture"))->SetMainTexture(texture);
	go = _Maker->Instantiate(L"testGo");
	go->AddComponent<CMeshRenderer>()->SetModel(_MeshFactory->SharedBuffer(EMeshType::Cube))->SetMaterial(material);
	go->SetLocalPosition(Vector3(-2, 1.0, 0));
	go->SetLocalScale(Vector3::one * 1);

	axis = _Maker->Instantiate(L"axis");
	axis->AddComponent<CMeshRenderer>()->SetModel(_MeshFactory->SharedBuffer(EMeshType::Cube))->SetMaterial(material);
	axis->SetLocalScale(Vector3(0.2f, 0.2f, 10.0f));
	CEditorTool::PrintTree();
}

void CRelationshipTest::OnUpdate()
{
	if (!go) return;
	float h = CInput::GetAxis("Horizontal") * CTime::deltaTime * 50;
	float v = CInput::GetAxis("Vertical") * CTime::deltaTime * 50;
	Vector3 euler = axis->GetLocalEulerAngles();
	euler.x += v;
	euler.y += h;
	axis->SetLocalEulerAngles(euler);
	
	Vector3 pos = go->GetLocalPosition();
	pos = Quaternion::AngleAxis(axis->GetForward(), CTime::deltaTime * 500) * pos;
	go->SetLocalPosition(pos);
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
		info->windowWidth = 800;
		info->windowHeight = 600;
		info->windowBits = 32;
		info->isFullScreen = false;
		info->appName = L"GameEngineTest";
		info->className = L"MainClass";
	}
}

void CRelationshipTest::InitLight()
{
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	//glLightfv(GL_LIGHT0, GL_POSITION, direction);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

