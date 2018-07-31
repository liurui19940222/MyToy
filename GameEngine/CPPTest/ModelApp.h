#pragma once
#include"GLAppBase\GLAppBase.h"
#include"SpRendering\GLRendering.h"
#include"SpRendering\FontRenderer.h"
#include"SpCommon\Input.h"
#include"SpAnimation\SkeletonAnimator.h"
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>


USING_NAMESPACE_ENGINE;
using namespace Assimp;

class ModelApp : public GLAppBase
{
	const int font_size = 14;
protected:
	virtual void InitGL(HWND hwnd) override;
	virtual void OnInitialize() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnWindowSizeChanged(int width, int height) override;
	virtual void OnRender() override;

public:
	ModelApp(const wchar_t* className, const wchar_t* title, int width, int height);

private:
	Matrix4x4				modelMat;
	Matrix4x4				viewMat;
	Matrix4x4				projectionMat;
	IRenderingInterface*	m_RI;
	RenderingObject			m_Object;
	PMeshBuffer				m_MeshBuffer;
	PMaterial				m_Material;
	Importer				m_Importer;
	PSkeleton				m_Skeleton;
	SkeletonAnimator		m_Animator;
	PMesh					m_Mesh;
	PAnimationClip			m_Clip;
	PAnimationClip			m_Clip2;
	Vector3					m_CameraPos;
};
