#include "ModelApp.h"
#include "..\SpRendering\MeshFactory.h"
#include "..\SpRendering\MeshBufferSkinning.h"
#include "..\SpAssetLoader\AdvModelLoader.h"

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

	modelMat = Matrix4x4::Translate(Vector3(0, -1, 0)) * Matrix4x4::Rotate(Vector3(-90, 0, 0)) * Matrix4x4::Scale(Vector3::one * 0.035f);
	m_CameraPos = Vector3(0, 2, 10);
	viewMat = Matrix4x4::LookAt(m_CameraPos, Vector3::zero, Vector3::up);

	m_Material = make_shared<Material>();
	m_Material->SetName(L"role_mat");
	m_Material->SetShader(Shader::Get("skinning"));
	m_Material->SetMainTexture(Texture2D::Create("../Assets/models/warrior/w2s_diffuse.tga"));

	AdvModelLoader loader;
	PModel model = loader.LoadFromFile("../Assets/models/warrior/w2s.FBX");
	m_Skeleton = model->m_Skeleton;

	PAnimationClip attack_clip = SkeletonAnimation::Slice(model->m_Animations[0], 100, 123, "attack");
	PAnimationClip idle_clip = SkeletonAnimation::Slice(model->m_Animations[0], 0, 50, "idle");
	PAnimationClip run_clip = SkeletonAnimation::Slice(model->m_Animations[0], 200, 220, "run");
	idle_clip->m_IsLooping = true;
	run_clip->m_IsLooping = true;

	PAnimationClip walk_clip = loader.LoadAnimationFromFile("../Assets/models/warrior/w2s_walk.FBX", *m_Skeleton.get());
	walk_clip->m_IsLooping = true;
	walk_clip->m_Name = "walk";

	PAnimationClip jump_clip = loader.LoadAnimationFromFile("../Assets/models/warrior/w2s_jump.FBX", *m_Skeleton.get());
	jump_clip->m_IsLooping = false;
	jump_clip->m_Name = "jump";
	jump_clip = SkeletonAnimation::Slice(jump_clip, 0, 16, "jump");

	m_MeshBuffer = make_shared<MeshBufferSkinning>(model->m_Meshes[0]);
	m_Object.mesh = m_MeshBuffer.get();
	m_Object.material = m_Material.get();

	m_Animator.AddClip(idle_clip);
	m_Animator.AddClip(walk_clip);
	m_Animator.AddClip(run_clip);
	m_Animator.AddClip(jump_clip);
	m_Animator.AddClip(attack_clip);
	m_Animator.SetSkeleton(m_Skeleton);
	m_Animator.Play("idle");

	//创建地面
	m_GroundMaterial = make_shared<Material>();
	m_GroundMaterial->SetName(L"ground_mat");
	m_GroundMaterial->SetShader(Shader::Get("texture"));
	m_GroundMaterial->SetMainTexture(Texture2D::Create("../Assets/textures/wooden_case.jpg"));
	m_GroundBuffer = _MeshFactory->CreateBuffer<MeshBufferTexcoord>(EMeshType::Quad);
	m_Ground.mesh = m_GroundBuffer.get();
	m_Ground.material = m_GroundMaterial.get();
	groundMat = Matrix4x4::Translate(Vector3(0, -2.75, 0)) * Matrix4x4::Rotate(Vector3(-90, 0, 0)) * Matrix4x4::Scale(Vector3::one * 15);

	//初始化粒子
	m_ParticleBuffer = _MeshFactory->CreateBuffer<MeshBufferParticle>(EMeshType::Quad);
	ParticleDesc desc;
	desc.m_AccelSpeed.set(1.0f, 1.5f);
	desc.m_Life.set(0.8f, 1.0f);
	desc.m_MaxNum = 100.0f;
	desc.m_Speed.set(2.0f, 1.5f);
	desc.m_Size.set(2.0f, 1.2f);
	desc.m_UseFrameAnim = true;
	desc.m_AnimCellX = 4;
	desc.m_AnimCellY = 4;
	desc.m_AnimSpeed = 50.0f;
	desc.m_Rate = 0.03f;
	desc.m_SizeSequence.insert(0.0f, 0.4f);
	desc.m_SizeSequence.insert(0.7f, 1.0f);
	desc.m_SizeSequence.insert(1.0f, 0.0f);
	desc.m_ColorSequence.insert(0.0f, Color(1.0f, 0.439f, 0.0f, 1.0f));
	desc.m_ColorSequence.insert(0.45f, Color(1.0f, 0.439f, 0.0f, 1.0f));
	desc.m_ColorSequence.insert(1.0f, Color(0.455f, 0.455f, 0.455f, 0.0f));

	m_ParticleMat = make_shared<Material>();
	m_ParticleMat->SetName(L"particle_mat");
	m_ParticleMat->SetShader(Shader::Get("particle_blend"));
	PTexture2D tex = Texture2D::Create("../Assets/textures/Tex_Flame.jpg");
	tex->SetWrapMode(ETexWrapMode::Repeat);
	m_ParticleMat->SetMainTexture(tex);
	m_ParticleMat->SetBlendFunc(EBlendFactor::SRC_ALPHA, EBlendFactor::ONE);
	m_ParticleMat->SetState(statetype::EPiplelineStateType::ZWrite, false);
	m_ParticleEffect = make_shared<ParticleEffect>(desc);
	m_ParticleEffect->CreateParticle();
	m_ParticleEffect->SetMaterial(m_ParticleMat);
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
		m_Animator.FadeIn("idle", 2.0f);
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
		m_Animator.FadeIn("walk", 2.0f);
	}

	if (Input::GetKeyDown(DIK_6))
	{
		m_Animator.FadeIn("run", 2.0f);
	}

	if (Input::GetKeyDown(DIK_SPACE))
	{
		m_Animator.FadeInOut("jump", 2.0f, 2.0f);
	}

	if (Input::GetKeyDown(DIK_K))
	{
		m_Animator.FadeInOut("attack", 2.0f, 2.0f);
	}

	m_Animator.OnUpdate(deltaTime);
	m_ParticleEffect->OnUpdate(deltaTime);
}

void ModelApp::OnRender()
{
	GLAppBase::OnRender();

	//m_Material->Bind();
	//m_Material->SetParam("GlobalPoseMatrices", &m_Skeleton->m_SkiningMatrices[0], m_Skeleton->GetSize());
	//m_Material->SetParam("M", modelMat);
	//m_Material->SetParam("V", viewMat);
	//m_Material->SetParam("P", projectionMat);
	//m_Material->SetParam("Color", Color::white);
	//m_RI->Render(m_Object);
	//m_Material->Unbind();

	m_GroundMaterial->Bind();
	m_GroundMaterial->SetParam("M", groundMat);
	m_GroundMaterial->SetParam("V", viewMat);
	m_GroundMaterial->SetParam("P", projectionMat);
	m_GroundMaterial->SetParam("Color", Color::white);
	m_RI->Render(m_Ground);
	m_GroundMaterial->Unbind();

	m_ParticleEffect->OnRender(*m_RI, *m_ParticleBuffer.get(), viewMat, projectionMat, m_CameraPos);
}
