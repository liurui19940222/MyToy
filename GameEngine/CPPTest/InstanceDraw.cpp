#include "InstanceDraw.h"

InstanceDraw::InstanceDraw(const wchar_t* className, const wchar_t* title, int width, int height)
	: GLAppBase(className, title, width, height) {}

void InstanceDraw::OnInitialize()
{
	GLAppBase::OnInitialize();
	GLfloat vertices[] = {
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	GLshort indices[] = {
		0, 1, 2,
		0, 2, 3,
	};

	GLfloat texcoords[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	m_InstanceMatrices.resize(INSTANCE_NUM);
	float x_pos = -INSTANCE_NUM * 0.5f + 0.5;
	for (int i = 0; i < INSTANCE_NUM; ++i)
	{
		m_InstanceMatrices[i] = Matrix4x4::Translate(Vector3(x_pos + i, 0.0f, 0.0f)) * Matrix4x4::Scale(Vector3::one * 0.5f);
	}

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VertexHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glGenBuffers(1, &m_TexcoordHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_TexcoordHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glGenBuffers(1, &m_ModelMatrixHandle);
	glBindBuffer(GL_ARRAY_BUFFER, m_ModelMatrixHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4x4) * INSTANCE_NUM, &m_InstanceMatrices[0], GL_DYNAMIC_DRAW);
	for (int i = 0; i < 4; ++i)
	{
		glVertexAttribPointer(MODEL_MATRIX_POS + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4x4), (void*)(sizeof(Vector4) * i));
		glEnableVertexAttribArray(MODEL_MATRIX_POS + i);
		glVertexAttribDivisor(MODEL_MATRIX_POS + i, 1);
	}

	glGenBuffers(1, &m_IndicesHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndicesHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	m_ViewMatrix = Matrix4x4::LookAt(Vector3(0, 0, 10), Vector3::zero, Vector3::up);
	m_Texture = Texture2D::Create("../Assets/star.png");
	m_Shader = Shader::Get("instance_draw");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void InstanceDraw::OnWindowSizeChanged(int width, int height)
{
	GLAppBase::OnWindowSizeChanged(width, height);
	m_ProjMatrix = Matrix4x4::Perspective(45.0f, width / (float)height, 0.01f, 1000.0f);
}

void InstanceDraw::OnUpdate(float deltaTime)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ModelMatrixHandle);
	Matrix4x4* matrices = (Matrix4x4*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	float x_pos = -INSTANCE_NUM * 0.5f + 0.5;
	for (int i = 0; i < INSTANCE_NUM; ++i)
	{
		m_Angle += deltaTime * 100;
		matrices[i] = Matrix4x4::Translate(Vector3(x_pos + i, 0.0f, 0.0f)) 
			* Matrix4x4::Rotate(0.0f, 0.0f, m_Angle * (!(i % 2) ? -1.0f : 1.0f)) * Matrix4x4::Scale(Vector3::one * 0.5f);
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstanceDraw::OnRender()
{
	GLAppBase::OnRender();

	m_ModelMatrix = Matrix4x4::Identity();

	m_Shader->Run();
	m_Shader->SetUniformParam("u_M", m_ModelMatrix);
	m_Shader->SetUniformParam("u_V", m_ViewMatrix);
	m_Shader->SetUniformParam("u_P", m_ProjMatrix);
	glActiveTexture(GL_TEXTURE0);
	m_Texture->Bind();
	m_Shader->SetUniformParam("u_MainTex", 0);

	glBindVertexArray(m_VAO);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, INSTANCE_NUM);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}