#include "ModelApp.h"
#include "SpRendering\MeshFactory.h"
#include "SpRendering\MeshBufferSkinning.h"
#include "SpAssetLoader\AdvModelLoader.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

using namespace std;

ModelApp::ModelApp(const wchar_t* className, const wchar_t* title, int width, int height)
	: GLAppBase(className, title, width, height) {}

void ModelApp::InitGL(HWND hwnd)
{
	GLRenderingStartUpParams params;
	params.m_HWND = hwnd;
	m_RI = new GLRendering();
	m_RI->StartUp(&params);
}

void ModelApp::OnInitialize()
{
	GLAppBase::OnInitialize();
	Color color = Color::Hex(0x314D79FF);
	SetBackgroundColor(color.r, color.g, color.b, color.a);
	Input::Init(GetModuleHandle(NULL), m_Hwnd);

	modelMat = Matrix4x4::Translate(Vector3(0, -3, 0)) * Matrix4x4::Rotate(Vector3(-90, 20, 0)) * Matrix4x4::Scale(Vector3::one * 0.5f);
	m_CameraPos = Vector3(0, 3, 10);
	viewMat = Matrix4x4::LookAt(m_CameraPos, Vector3::zero, Vector3::up);

	m_Material = make_shared<Material>();
	m_Material->SetShader(Shader::Get("skinning"));
	//m_Material->SetMainTexture(Texture2D::Create("../Assets/shake.png"));
	m_Material->SetMainTexture(Texture2D::Create("D:/project/client_branch_1.3.5/UnityProj/Assets/ArtRawResources/Actors/basktball_Boy/Textures/2000.png"));

	AdvModelLoader loader;
	//loader.LoadFromFile("../Assets/models/shake.xml");
	loader.LoadFromFile("D:/1100.FBX");
	m_Mesh = loader.m_model->m_Meshes[0];
	m_Clip = loader.m_model->m_Animations[0];
	m_Clip->m_IsLooping = true;
	m_Skeleton = loader.m_model->m_Skeleton;

	m_MeshBuffer = make_shared<MeshBufferSkinning>(m_Mesh);
	m_Object.mesh = m_MeshBuffer.get();
	m_Object.material = m_Material.get();
}

void ModelApp::OnWindowSizeChanged(int width, int height)
{
	GLAppBase::OnWindowSizeChanged(width, height);
	projectionMat = Matrix4x4::Perspective(45, width / (float)height, 0.1, 1000);
}

float g_DeltaTime;
float g_Time;
void ModelApp::OnUpdate(float deltaTime)
{
	GLAppBase::OnUpdate(deltaTime);
	Input::GetState(EngineToolkit::GetGlobalRect(m_Hwnd, m_WindowWidth, m_WindowHeight));

	g_DeltaTime = deltaTime;
	EngineToolkit::WatchTarget(m_CameraPos, viewMat, Vector3::zero, g_DeltaTime);

	vector<JointPose> jointPoses = SkeletonAnimation::Sample(m_Clip, m_Skeleton, this->m_TimeSinceStarUp, 1);
	SkeletonAnimation::CalculateGlobalMatrix(m_Skeleton, jointPoses);
	SkeletonAnimation::CalculateSkinningMatrix(m_Skeleton);
}

void ModelApp::OnRender()
{
	GLAppBase::OnRender();

	m_Material->Bind();
	m_Material->SetParam("GlobalPoseMatrices", &m_Skeleton->m_SkiningMatrices[0], m_Skeleton->GetSize());
	m_Material->SetParam("M", modelMat);
	m_Material->SetParam("V", viewMat);
	m_Material->SetParam("P", projectionMat);
	m_Material->SetParam("Color", Color::white);
	m_RI->Render(m_Object);
	m_Material->Unbind();
}
