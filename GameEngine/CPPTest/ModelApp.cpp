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

	modelMat = Matrix4x4::Translate(Vector3(0, -1, 0)) * Matrix4x4::Rotate(Vector3(-90, 20, 0)) * Matrix4x4::Scale(Vector3::one * 0.05f);
	m_CameraPos = Vector3(0, 3, 10);
	viewMat = Matrix4x4::LookAt(m_CameraPos, Vector3::zero, Vector3::up);

	m_Material = make_shared<Material>();
	m_Material->SetShader(Shader::Get("skinning"));
	m_Material->SetMainTexture(Texture2D::Create("../Assets/models/warrior/w2s_diffuse.tga"));

	AdvModelLoader loader;
	PModel model = loader.LoadFromFile("../Assets/models/warrior/w2s.FBX");
	m_Mesh = model->m_Meshes[0];
	m_Skeleton = model->m_Skeleton;

	PModel model2 = loader.LoadAnimationFromFile("../Assets/models/warrior/w2s_walk.FBX", *m_Skeleton.get());
	m_Clip = model2->m_Animations[0];
	m_Clip->m_IsLooping = true;
	m_Clip->m_Name = "walk";

	PModel model3 = loader.LoadAnimationFromFile("../Assets/models/warrior/w2s_jump.FBX", *m_Skeleton.get());
	m_Clip2 = model3->m_Animations[0];
	m_Clip2->m_IsLooping = false;
	m_Clip2->m_Name = "jump";

	m_MeshBuffer = make_shared<MeshBufferSkinning>(m_Mesh);
	m_Object.mesh = m_MeshBuffer.get();
	m_Object.material = m_Material.get();

	m_Animator.AddClip(m_Clip);
	m_Animator.AddClip(m_Clip2);
	m_Animator.SetSkeleton(m_Skeleton);
	m_Animator.Play("walk");
}

void ModelApp::OnWindowSizeChanged(int width, int height)
{
	GLAppBase::OnWindowSizeChanged(width, height);
	projectionMat = Matrix4x4::Perspective(45, width / (float)height, 0.1, 1000);
}

void ModelApp::OnUpdate(float deltaTime)
{
	GLAppBase::OnUpdate(deltaTime);
	Input::GetState(EngineToolkit::GetGlobalRect(m_Hwnd, m_WindowWidth, m_WindowHeight));

	EngineToolkit::WatchTarget(m_CameraPos, viewMat, Vector3::zero, deltaTime);

	if (Input::GetKeyDown(DIK_1))
	{
		m_Animator.Play("walk");
	}

	if (Input::GetKeyDown(DIK_2))
	{
		m_Animator.Pause();
	}

	if (Input::GetKeyDown(DIK_3))
	{
		m_Animator.Continue();
	}

	if (Input::GetKeyDown(DIK_4))
	{
		m_Animator.Stop();
	}

	if (Input::GetKeyDown(DIK_5))
	{
		m_Animator.FadeIn("jump", 2.0f);
	}

	if (Input::GetKeyDown(DIK_6))
	{
		m_Animator.FadeInOut("jump", 2.0f, 2.0f);
	}

	m_Animator.OnUpdate(deltaTime);
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
