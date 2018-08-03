#pragma once

#include"GLAppBase\GLAppBase.h"
#include"..\SpRendering\Shader.h"
#include"..\SpRendering\Material.h"
#include"..\SpRendering\Texture2D.h"

USING_NAMESPACE_ENGINE;

class InstanceDraw : public GLAppBase
{
	const int INSTANCE_NUM = 10;
	const int MODEL_MATRIX_POS = 3;
public:
	InstanceDraw(const wchar_t* className, const wchar_t* title, int width, int height);

protected:
	virtual void OnInitialize() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnWindowSizeChanged(int width, int height) override;

private:
	GLuint			m_VAO;
	GLuint			m_VertexHandle;
	GLuint			m_TexcoordHandle;
	GLuint			m_TexcoordRangeHandle;
	GLuint			m_ModelMatrixHandle;
	GLuint			m_IndicesHandle;
	PShader			m_Shader;
	PTexture		m_Texture;
	Matrix4x4		m_ModelMatrix;
	Matrix4x4		m_ViewMatrix;
	Matrix4x4		m_ProjMatrix;
	float			m_Angle;
	vector<TexcoordRange>	m_InstanceTexcoordRange;
	vector<Matrix4x4>		m_InstanceMatrices;
};

