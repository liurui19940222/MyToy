#include "TestTest.h"
#include "MeshFactory.h"

void CTestTest::OnStart() {
	_MainCameraGo->SetLocalPosition(Vector3(0, 2, -10));
	go = _Maker->Instantiate("NewGameObject");
	CMaterial* mat = _Maker->Instantiate<CMaterial>();
	mat->SetMainTexture(CTexture2D::Create("textures/dlg01.bmp"))->SetShader(CShader::Get("texture"));
	go->AddComponent<CMeshRenderer>()->SetModel(_MeshFactory->SharedBuffer(EMeshType::Cube))
		->SetMaterial(mat);

	_MainCameraGo->LookAt(go->GetLocalPosition());

	Quaternion q1 = Quaternion::AngleAxis(Vector3::up, 20);
	Quaternion q2 = Quaternion::AngleAxis(Vector3::right, 20);
	Quaternion q3 = q1 * q2;
	CDebug::Log(q1);
	CDebug::Log(q2);
	CDebug::Log(q3);
	Matrix4x4 matrix;
	matrix.MakeRotate(q3);
	matrix = Matrix4x4::Translate(Vector3::zero) * matrix * Matrix4x4::Scale(Vector3::one);
	CDebug::Log(matrix);
}

void CTestTest::OnUpdate() 
{
	static float axis_y = 0, axis_z = 0;
	float h = CInput::GetAxis("Horizontal") * CTime::deltaTime * 100;
	float v = CInput::GetAxis("Vertical") * CTime::deltaTime * 100;
	axis_y += h;
	axis_z += v;
	go->SetRotation(Quaternion::AngleAxis(Vector3::right, axis_z));

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