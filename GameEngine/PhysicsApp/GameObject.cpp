#include "GameObject.h"

GameObject::GameObject() : m_Scale(Vector3::one)
{
}

GameObject::~GameObject()
{
}

void GameObject::OnRender(IRenderingInterface& ri, const Matrix4x4& v, const Matrix4x4& p)
{
	if (!m_Material || !m_MeshBuffer)
		return;
	m_Material->Bind();
	GetModelMatrix(m_ModelMatrix);
	m_Material->SetParam("M", m_ModelMatrix);
	m_Material->SetParam("V", v);
	m_Material->SetParam("P", p);
	ri.Render(RenderingObject{ m_MeshBuffer.get(), m_Material.get() });
}

void GameObject::OnUpdate(float deltaTime)
{
	
}

void GameObject::GetModelMatrix(Matrix4x4& out) const
{
	static Matrix4x4 translate, scale, rotation;
	translate.MakeTranslate(m_Position);
	scale.MakeScale(m_Scale);
	rotation.MakeRotate(m_Rotation);
	out = translate * scale * rotation;
}