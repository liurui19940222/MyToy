#include "TestTest.h"
#include "MeshFactory.h"

void CTestTest::OnStart() {
	_MainCameraGo->SetLocalPosition(Vector3(0, 2, -10));
	go = _Maker->Instantiate("NewGameObject");
	go2 = _Maker->Instantiate("2");
	CMaterial* mat = _Maker->Instantiate<CMaterial>();
	mat->SetMainTexture(CTexture2D::Create("textures/dlg01.bmp"))->SetShader(CShader::Get("texture"));
	go->AddComponent<CMeshRenderer>()->SetModel(_MeshFactory->SharedBuffer(EMeshType::Cube))
		->SetMaterial(mat);

	go2->AddComponent<CMeshRenderer>()->SetModel(_MeshFactory->SharedBuffer(EMeshType::Cube))
		->SetMaterial(mat);
	go2->SetLocalPosition(Vector3(2, 0, 0));

	_MainCameraGo->LookAt(go->GetLocalPosition());

	Quaternion q1 = Quaternion::AngleAxis(Vector3::up, 20);
	Quaternion q2 = Quaternion::AngleAxis(Vector3::right, 20);
	Quaternion q3 = q1 * q2;
	CDebug::Log(q1);
	CDebug::Log(q2);
	CDebug::Log(q3);
	Matrix4x4 matrix;
	matrix.MakeRotate(q3);
	CDebug::Log(matrix);
	Quaternion q4 = Matrix4x4::ToQuaternion(matrix);
	CDebug::Log(q4);
}

void CTestTest::OnUpdate() 
{
	float h = CInput::GetAxis("Horizontal") * CTime::deltaTime * 100;
	float v = CInput::GetAxis("Vertical") * CTime::deltaTime * 100;

	Vector3 eul = go->GetLocalEulerAngles();
	eul.y += h;
	eul.z += v;
	go->SetLocalEulerAngles(eul);
	go->SetRotation(go->GetRotation());
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