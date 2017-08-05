#include"MeshRenderer.h"
#include"GameObject.h"
#include"EngineDefine.h"
#include"Engine.h"
#include"Application.h"

IMPL_CLASS(CMeshRenderer)

CMeshRenderer::CMeshRenderer()
{

}


CMeshRenderer::~CMeshRenderer()
{
}

void CMeshRenderer::OnUpdate()
{

}

void CMeshRenderer::OnRender()
{
	if (!model) return;

	if (texture)
	{
		glEnable(GL_TEXTURE_2D);
		texture->Bind();
	}
	CShader* shader = CShader::Get("texture");
	shader->Run();
	shader->SetUniformParam("MVP", MainCamera->GetProjectionMatrix() * MainCamera->GetViewMatrix() * gameObject->GetModelToWorldMat());
	shader->SetUniformParam("Color", Color::white);
	shader->SetUniformParam("MainTex", 0);
	model->GetBuffer()->BindBuffer();
	glDrawArrays(model->GetGLMode(), 0, model->GetVertexNum());
	glUseProgram(0);
	glBindVertexArray(0);
	if (texture)
	{
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void CMeshRenderer::OnDestroy()
{

}

CMeshRenderer* CMeshRenderer::SetModel(MeshProvider* model)
{
	this->model = model;
	return this;
}

CMeshRenderer* CMeshRenderer::SetTexture(CTexture* texture)
{
	this->texture = texture;
	return this;
}