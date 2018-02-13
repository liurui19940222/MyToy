#include "TestTest.h"
#include"SpRendering\MeshFactory.h"
#include<memory>

void CTestTest::OnStart() {
	_MainCameraGo->SetLocalPosition(Vector3(0, 2, -10));
	go = _Maker->Instantiate(L"NewGameObject");
	go2 = _Maker->Instantiate(L"2");
	PMaterial mat = make_shared<Material>();
	mat->SetMainTexture(Texture2D::Create("textures/dlg01.bmp"))->SetShader(Shader::Get("texture"));
	go->AddComponent<CMeshRenderer>()->SetModel(_MeshFactory->SharedBuffer(EMeshType::Cube))
		->SetMaterial(mat);

	go2->AddComponent<CMeshRenderer>()->SetModel(_MeshFactory->SharedBuffer(EMeshType::Cube))
		->SetMaterial(mat);
	go2->SetLocalPosition(Vector3(2, 0, 0));

	_MainCameraGo->LookAt(go->GetLocalPosition());

	Quaternion q1 = Quaternion::AngleAxis(Vector3::up, 20);
	Quaternion q2 = Quaternion::AngleAxis(Vector3::right, 20);
	Quaternion q3 = q1 * q2;
	Debug::Log(q1);
	Debug::Log(q2);
	Debug::Log(q3);
	Matrix4x4 matrix;
	matrix.MakeRotate(q3);
	Debug::Log(matrix);
	Quaternion q5 = Quaternion::LookRotation(Vector3(-9.99, 20.66, -9.3), Vector3::up);
	Debug::Log(q5);
	Quaternion q6 = Quaternion::Lerp(q1 * q3, q2 * q3, 0.99f);
	Debug::Log(q6);
}

void CTestTest::OnUpdate() 
{
	static float axis_x = 0, axis_y = 0, axis_z = 0;
	float h = CInput::GetAxis("Horizontal") * CTime::deltaTime * 10;
	float v = CInput::GetAxis("Vertical") * CTime::deltaTime * 10;
	axis_y += h;
	axis_z += v;
	if (CInput::GetKey(DIK_I))
	{
		axis_x += CTime::deltaTime * 10;
	}
	if (CInput::GetKey(DIK_K))
	{
		axis_x -= CTime::deltaTime * 10;
	}
	go2->SetLocalPosition(Vector3(-axis_y, axis_z, axis_x));
	
	if (!Vector3::Approximately(go2->GetLocalPosition(), go->GetLocalPosition()))
	{
		Quaternion target = Quaternion::LookRotation(go2->GetLocalPosition() - go->GetLocalPosition(), Vector3::up);
		go->SetRotation(Quaternion::Slerp(go->GetRotation(), target, CTime::deltaTime * 10));
	}
	CEditorTool::WatchTarget(*_MainCameraGo, go->GetLocalPosition());
}

void CTestTest::OnRender() {}

void CTestTest::OnClose() {}

void CTestTest::GetApplicationInfo(SApplicationInfo* info) {
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