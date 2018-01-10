#include"SkyBox.h"
#include"Maker.h"
#include"Shader.h"

CSkyBox* CSkyBox::Create(const char* top, const char* bottom, const char* front, const char* back, const char* left, const char* right)
{
	CSkyBox* box = _Maker->Instantiate<CSkyBox>();
	box->m_textures[(int)ESkyBoxFace::Top] = CTexture2D::Create(top, ETexWrapMode::Clamp, ETexFilterMode::Point, ETexEnvMode::Replace, true);
	box->m_textures[(int)ESkyBoxFace::Bottom] = CTexture2D::Create(bottom, ETexWrapMode::Clamp, ETexFilterMode::Point, ETexEnvMode::Replace, true);
	box->m_textures[(int)ESkyBoxFace::Front] = CTexture2D::Create(front, ETexWrapMode::Clamp, ETexFilterMode::Point, ETexEnvMode::Replace, true);
	box->m_textures[(int)ESkyBoxFace::Back] = CTexture2D::Create(back, ETexWrapMode::Clamp, ETexFilterMode::Point, ETexEnvMode::Replace, true);
	box->m_textures[(int)ESkyBoxFace::Left] = CTexture2D::Create(left, ETexWrapMode::Clamp, ETexFilterMode::Point, ETexEnvMode::Replace, true);
	box->m_textures[(int)ESkyBoxFace::Right] = CTexture2D::Create(right, ETexWrapMode::Clamp, ETexFilterMode::Point, ETexEnvMode::Replace, true);
	return box;
}

void CSkyBox::OnInitialize()
{
	//front
	Vector3 front[] = {
		{ -0.5f, 0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f, -0.5f },
		{ -0.5f, 0.5f, -0.5f },
		{ 0.5f, -0.5f, -0.5f },
		{ 0.5f, 0.5f, -0.5f },
	};
	//back
	Vector3 back[] = {
		{ 0.5f, 0.5f, 0.5f },
		{ 0.5f, -0.5f, 0.5f },
		{ -0.5f, -0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f },
		{ -0.5f, -0.5f, 0.5f },
		{ -0.5f, 0.5f, 0.5f },
	};
	//left
	Vector3 left[] = {
		{ -0.5f, 0.5f, 0.5f },
		{ -0.5f, -0.5f, 0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f, 0.5f, 0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f, 0.5f, -0.5f },
	};
	//right
	Vector3 right[] = {
		{ 0.5f, 0.5f, -0.5f },
		{ 0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f, 0.5f },
		{ 0.5f, 0.5f, -0.5f },
		{ 0.5f, -0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f },
	};
	//top
	Vector3 top[] = {
		{ -0.5f, 0.5f, 0.5f },
		{ -0.5f, 0.5f, -0.5f },
		{ 0.5f, 0.5f, -0.5f },
		{ -0.5f, 0.5f, 0.5f },
		{ 0.5f, 0.5f, -0.5f },
		{ 0.5f, 0.5f, 0.5f },
	};
	//bottom
	Vector3 bottom[] = {
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f, -0.5f, 0.5f },
		{ 0.5f, -0.5f, 0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f, 0.5f },
		{ 0.5f, -0.5f, -0.5f },
	};
	Vector2 uv[] = {
		Vector2(0.0f, 1.0f),
		Vector2(0.0f, 0.0f),
		Vector2(1.0f, 0.0f),
		Vector2(0.0f, 1.0f),
		Vector2(1.0f, 0.0f),
		Vector2(1.0f, 1.0f),
	};
	m_buffer[(int)ESkyBoxFace::Top].MakeBuffer(top, NULL, NULL, uv, 6);
	m_buffer[(int)ESkyBoxFace::Bottom].MakeBuffer(bottom, NULL, NULL, uv, 6);
	m_buffer[(int)ESkyBoxFace::Front].MakeBuffer(front, NULL, NULL, uv, 6);
	m_buffer[(int)ESkyBoxFace::Back].MakeBuffer(back, NULL, NULL, uv, 6);
	m_buffer[(int)ESkyBoxFace::Left].MakeBuffer(left, NULL, NULL, uv, 6);
	m_buffer[(int)ESkyBoxFace::Right].MakeBuffer(right, NULL, NULL, uv, 6);
}

void CSkyBox::OnRelease()
{
	for (int i = 0; i < 6; ++i)
	{
		_Maker->Destroy(m_textures[i]);
		m_buffer[i].ReleaseBuffer();
	}
}

void CSkyBox::Render(const Matrix4x4& model, const Matrix4x4& view, const Matrix4x4& projection)
{
	glDisable(GL_DEPTH_TEST);

	CShader* shader = CShader::Get("texture");
	for (int i = 0; i < 6; ++i)
	{
		if (!m_textures[i]) continue;
		shader->Run();
		m_textures[i]->Bind();
		shader->SetUniformParam("Color", Color::white);
		shader->SetUniformParam("MainTex", 0);
		shader->SetUniformParam("M", model);
		shader->SetUniformParam("V", view);
		shader->SetUniformParam("P", projection);
		m_buffer[i].BindBuffer();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
}