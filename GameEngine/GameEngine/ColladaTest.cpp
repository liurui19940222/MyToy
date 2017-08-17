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
	model->SetLocalScale(Vector3(0.1f, 0.1f, 0.1f) * 4);
	model->SetLocalEulerAngles(Vector3(0, 180, 0));
	CMaterial* model_mat = _Maker->Instantiate<CMaterial>()->SetShader(CShader::Get("light"));
	//model->AddComponent<CMeshRenderer>()->SetModel(_Maker->Instantiate<CMeshCube>())->SetMaterial(model_mat);
	collada = _Resources->Load<CColladaFile>("models/walk.dae");
	model->AddComponent<CSkinnedMeshRenderer>()->SetModel(collada)->SetMaterial(model_mat);
	//glm::mat4 m;
	//m[0][0] = 2.54; m[1][0] = -0; 		m[2][0] = 0; 			m[3][0] = 0;
	//m[0][1] = 0; 	m[1][1] = 2.52908;  m[2][1] = -0.235263; 	m[3][1] = 0;
	//m[0][2] = 0; 	m[1][2] = 0.235263; m[2][2] = 2.52908; 		m[3][2] = 0;
	//m[0][3] = -0;	m[1][3] = -6.84199; m[2][3] = 0.987113; 	m[3][3] = 1;

	//Matrix4x4 ma;
	//ma[0][0] = 2.54; ma[1][0] = -0; 		ma[2][0] = 0; 			ma[3][0] = 0;
	//ma[0][1] = 0; 	ma[1][1] = 2.52908;  ma[2][1] = -0.235263; 	ma[3][1] = 0;
	//ma[0][2] = 0; 	ma[1][2] = 0.235263; ma[2][2] = 2.52908; 		ma[3][2] = 0;
	//ma[0][3] = -0;	ma[1][3] = -6.84199; ma[2][3] = 0.987113; 	ma[3][3] = 1;

	//glm::mat4 result1 = m * m._inverse();
	//glm::mat4 result2 = m._inverse() * m;

	//Matrix4x4 result3 = ma * ma.Inverse();
	//Matrix4x4 result4 = ma.Inverse() * ma;

	//int j = 0;
	Matrix4x4 trans = Matrix4x4::Translate(Vector3(30, 0, 0));
	Matrix4x4 inv = trans.Inverse();
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