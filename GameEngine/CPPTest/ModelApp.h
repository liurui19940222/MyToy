#pragma once
#include"GLAppBase\GLAppBase.h"
#include"..\SpRendering\GLRendering.h"
#include"..\SpRendering\FontRenderer.h"
#include"..\SpCommon\Input.h"
#include"..\SpAnimation\SkeletonAnimator.h"
#include"..\SpRendering\ParticleEffect.h"

USING_NAMESPACE_ENGINE;

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
	Matrix4x4				groundMat;
	Matrix4x4				viewMat;
	Matrix4x4				projectionMat;
	IRenderingInterface*	m_RI;
	RenderingObject			m_Object;
	RenderingObject			m_Ground;
	MeshBufferPtr			m_MeshBuffer;
	MeshBufferPtr			m_GroundBuffer;
	MaterialPtr				m_Material;
	MaterialPtr				m_GroundMaterial;
	SkeletonPtr				m_Skeleton;
	SkeletonAnimator		m_Animator;
	Vector3					m_CameraPos;

	ParticleEffectPtr		m_ParticleEffect;
	MeshBufferParticlePtr	m_ParticleBuffer;
	MaterialPtr				m_ParticleMat;
};
