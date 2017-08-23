#include"ColladaTest.h"
#include"ColladaFile.h"
#include"MeshCube.h"
#include"Maker.h"
#include<glm\glm.hpp>

void CColladaTest::OnStart()
{
	_MainCamera->SetCameraClearFlag(ECameraClearFlag::SolidColor);
	_MainCamera->SetCameraClearColor(Color::Hex(0x314D79FF));
	_MainCameraGo->SetLocalPosition(Vector3(0, 7, 10));
	_MainCameraGo->SetLocalEulerAngles(Vector3(-30, 180, 0));

	model = _Maker->Instantiate("model");
	model->SetLocalScale(Vector3(0.1f, 0.1f, 0.1f) * 10);
	model->SetLocalEulerAngles(Vector3(0, 180, 0));
	CMaterial* model_mat = _Maker->Instantiate<CMaterial>()->SetShader(CShader::Get("light"));
	collada = _Resources->Load<CColladaFile>("models/walk.dae");
	model->AddComponent<CSkinnedMeshRenderer>()->SetModel(collada)->SetMaterial(model_mat);

	Matrix4x4 m1;
	m1[0][0] = 1; m1[1][0] = 1.5; m1[2][0] = -5; m1[3][0] = 10;
	m1[0][1] = 0; m1[1][1] = 2; m1[2][1] = -5; m1[3][1] = 8;
	m1[0][2] = 8; m1[1][2] = 1.2; m1[2][2] = -3; m1[3][2] = 7;
	m1[0][3] = 9; m1[1][3] = 1.1; m1[2][3] = -0.5; m1[3][3] = 6;

	m1 = m1 * 5.0;
	m1 += m1;

	glm::mat4 m2;
	m2[0][0] = 1; m2[1][0] = 1.5; m2[2][0] = -5; m2[3][0] = 10;
	m2[0][1] = 0; m2[1][1] = 2; m2[2][1] = -5; m2[3][1] = 8;
	m2[0][2] = 8; m2[1][2] = 1.2; m2[2][2] = -3; m2[3][2] = 7;
	m2[0][3] = 9; m2[1][3] = 1.1; m2[2][3] = -0.5; m2[3][3] = 6;

	m2 = m2 * 5.0;
	m2 += m2;

	int j = 0;
}

void CColladaTest::OnUpdate()
{
	Vector3 euler = model->GetLocalEulerAngles();
	euler.y += CTime::deltaTime * 50;
	model->SetLocalEulerAngles(euler);

	float h = CInput::GetAxis("Horizontal") * CTime::deltaTime * 20;
	float v = CInput::GetAxis("Vertical") * CTime::deltaTime * 20;
	Vector3 position = model->GetLocalPosition();
	position.x += h;
	position.y += v;
	if (CInput::GetKey(DIK_NUMPAD2))
	{
		position.z += CTime::deltaTime * 20;
	}
	if (CInput::GetKey(DIK_NUMPAD8))
	{
		position.z -= CTime::deltaTime * 20;
	}
	model->SetLocalPosition(position);
}

void CColladaTest::OnRender()
{

}

void CColladaTest::OnClose()
{

}

void CColladaTest::GetApplicationInfo(SApplicationInfo* info)
{
	if (info)
	{
		info->windowWidth = _Config->GetValue<int>("window", "width");
		info->windowHeight = _Config->GetValue<int>("window", "height");
		info->isFullScreen = _Config->GetValue<bool>("window", "full_screen");
		info->windowBits = 32;
		info->appName = L"GameEngineTest";
		info->className = L"MainClass";
	}
}