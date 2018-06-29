#include "ModelApp.h"
#include "SpRendering\MeshFactory.h"
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

	modelMat = Matrix4x4::Rotate(Vector3(-90, 20, 0)) * Matrix4x4::Scale(Vector3::one * 0.5f);
	viewMat = Matrix4x4::LookAt(Vector3(0, 0, 10), Vector3::zero, Vector3::up);

	m_Material = make_shared<Material>();
	m_Material->SetShader(Shader::Get("texture"));
	m_Material->SetMainTexture(Texture2D::Create("D:/project/client_branch_1.3.5/UnityProj/Assets/ArtRawResources/Actors/eyu/Textures/eyu.png"));

	////const aiScene* scene = m_Importer.ReadFile("../SpGameEngine/models/shake.xml", aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	//const aiScene* scene = m_Importer.ReadFile("D:/project/client_branch_1.3.5/UnityProj/Assets/ArtRawResources/Actors/eyu/13000.FBX", aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	//m_Mesh = make_shared<Mesh>();
	//m_Mesh->m_Vertices = (Vector3*)scene->mMeshes[0]->mVertices;
	//m_Mesh->m_VertexCount = scene->mMeshes[0]->mNumVertices;
	//m_Mesh->m_Indices = (ushort*)malloc(sizeof(ushort) * scene->mMeshes[0]->mNumFaces * 3);
	//m_Mesh->m_TriangleCount = scene->mMeshes[0]->mNumFaces;
	//m_Mesh->m_Texcoords = (Vector2*)malloc(sizeof(Vector2) * scene->mMeshes[0]->mNumVertices);
	//for (int i = 0; i < scene->mMeshes[0]->mNumFaces; ++i)
	//{
	//	m_Mesh->m_Indices[i * 3 + 0] = scene->mMeshes[0]->mFaces[i].mIndices[0];
	//	m_Mesh->m_Indices[i * 3 + 1] = scene->mMeshes[0]->mFaces[i].mIndices[1];
	//	m_Mesh->m_Indices[i * 3 + 2] = scene->mMeshes[0]->mFaces[i].mIndices[2];
	//}
	//for (int i = 0; i < scene->mMeshes[0]->mNumVertices; ++i)
	//{
	//	m_Mesh->m_Texcoords[i].x = scene->mMeshes[0]->mTextureCoords[0][i].x;
	//	m_Mesh->m_Texcoords[i].y = scene->mMeshes[0]->mTextureCoords[0][i].y;
	//}

	AdvModelLoader loader;
	loader.LoadFromFile("D:/project/client_branch_1.3.5/UnityProj/Assets/ArtRawResources/Actors/eyu/13000.FBX");

	m_MeshBuffer = make_shared<MeshBufferTexcoord>(loader.m_model->m_Meshes[0]);
	m_Object.mesh = m_MeshBuffer.get();
	m_Object.material = m_Material.get();
}

void ModelApp::OnWindowSizeChanged(int width, int height)
{
	GLAppBase::OnWindowSizeChanged(width, height);
	projectionMat = Matrix4x4::Perspective(45, width / (float)height, 0.1, 1000);
}

void ModelApp::OnUpdate(float deltaTime)
{
	GLAppBase::OnUpdate(deltaTime);
}

void ModelApp::OnRender()
{
	GLAppBase::OnRender();

	m_Material->Bind();
	m_Material->SetParam("M", modelMat);
	m_Material->SetParam("V", viewMat);
	m_Material->SetParam("P", projectionMat);
	m_Material->SetParam("Color", Color::white);
	m_RI->Render(m_Object);
	m_Material->Unbind();
}
