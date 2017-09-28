#ifndef _GUI_MESH_H_
#define _GUI_MESH_H_

#include"GUIElement.h"

class CGUIMesh : public CGUIElement
{
private:
	Vector3* m_vertices;
	Vector3 m_eulerAngles;
	Vector3 m_scale;
	float m_zPos;
	int m_verticesCount;
	Color m_meshColor;

public:
	CGUIMesh();
	virtual void OnRender() override;
	CGUIMesh* SetVertices(Vector3* vertices, int count);
	CGUIMesh* SetMeshColor(const Color& color);
	CGUIMesh* SetEulerAngles(const Vector3& euler);
	CGUIMesh* SetScale(const Vector3& scale);
	CGUIMesh* SetZPos(float z);
};

#endif