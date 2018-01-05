#include"GUIMesh.h"
#include"GUIManager.h"

CGUIMesh::CGUIMesh() : CGUIElement(), m_vertices(NULL), m_verticesCount(0) 
{
	m_scale.x = m_scale.y = m_scale.z = 1.0f;
}

void CGUIMesh::OnRender()
{
	CGUIElement::OnRender();
	if (m_verticesCount > 0)
	{
		m_manager->GetRenderer()->DrawMesh(m_vertices, m_verticesCount,
			Matrix4x4::Translate(Vector3(m_position.x, m_position.y, m_zPos)) * Matrix4x4::Rotate(m_eulerAngles.x, m_eulerAngles.y, m_eulerAngles.z) * Matrix4x4::Scale(m_scale), m_meshColor);
	}
}

CGUIMesh* CGUIMesh::SetVertices(Vector3* vertices, int count)
{
	m_vertices = vertices;
	m_verticesCount = count;
	return this;
}

CGUIMesh* CGUIMesh::SetMeshColor(const Color& color)
{
	m_meshColor = color;
	return this;
}

CGUIMesh* CGUIMesh::SetEulerAngles(const Vector3& euler)
{
	m_eulerAngles = euler;
	return this;
}

CGUIMesh* CGUIMesh::SetScale(const Vector3& scale)
{
	m_scale = scale;
	return this;
}

CGUIMesh* CGUIMesh::SetZPos(float z)
{
	m_zPos = z;
	return this;
}