#ifndef _MESH_H_
#define _MESH_H_

#include"Math.h"
#include"Object.h"

class CMesh : public Object
{
private:
	Vector3* m_vertices;
	Vector3* m_normals;
	Vector2* m_texcoords;
	Vector3* m_colors;

public:

};

#endif