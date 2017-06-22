#include"MeshRenderer.h"
#include"GameObject.h"

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
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, (void*)model->GetVertexArray());
	glNormalPointer(GL_FLOAT, 0, model->GetNormalArray());
	glTexCoordPointer(2, GL_FLOAT, 0, model->GetUVArray());
	glDrawArrays(GL_TRIANGLES, 0, model->GetVertexNum());

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	if (texture)
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
	}
}

void CMeshRenderer::OnDestroy()
{

}

CMeshRenderer* CMeshRenderer::SetModel(IMeshProvider* model)
{
	this->model = model;
	return this;
}

CMeshRenderer* CMeshRenderer::SetTexture(CTexture* texture)
{
	this->texture = texture;
	return this;
}