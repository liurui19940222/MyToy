#include"EditorTool.h"
#include"Engine.h"
#include"Application.h"
#include<Windows.h>
#include<gl\GL.h>
#include<gl\GLU.h>

CEditorTool::CEditorTool()
{
}


CEditorTool::~CEditorTool()
{
}

void CEditorTool::DrawQuad(Vector3 position, float size)
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	float halfSize = size * 0.5f;
	glBegin(GL_QUADS);
	glVertex3f(-halfSize, halfSize, 0);
	glVertex3f(-halfSize, -halfSize, 0);
	glVertex3f(halfSize, -halfSize, 0);
	glVertex3f(halfSize, halfSize, 0);
	glEnd();
	glPopMatrix();
}

void CEditorTool::DrawGrid(Vector3 cameraPos, Vector3 pos, Color color)
{
	static float cellSize = 1.0f;
	static float cellCount = 10000.0f;
	static float length = cellSize * cellCount;
	static float cellLen = 0;

	glEnable(GL_FOG);
	glFogf(GL_FOG_DENSITY, 0.03f);
	glPushMatrix();
	glColor3f(color.r, color.g, color.b);
	glTranslatef(pos.x - length * 0.5f, pos.y, pos.z + length * 0.5f);

	glBegin(GL_LINES);
	for (float i = 0; i <= cellCount; i++)
	{
		cellLen = i * cellSize;
		glVertex3f(cellLen, 0, 0);
		glVertex3f(cellLen, 0, -length);
		glVertex3f(0, 0, -cellLen);
		glVertex3f(length, 0, -cellLen);
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_FOG);
}

void CEditorTool::DrawRect(SRect2D rect, Matrix4x4& modelToWorldMatrix)
{
	static Vector3 vertices[4];
	vertices[0].x = -rect.half_size_x; vertices[0].y = rect.half_size_y; vertices[0].z = 0;
	vertices[1].x = -rect.half_size_x; vertices[1].y = -rect.half_size_y; vertices[1].z = 0;
	vertices[2].x = rect.half_size_x; vertices[2].y = -rect.half_size_y; vertices[2].z = 0;
	vertices[3].x = rect.half_size_x; vertices[3].y = rect.half_size_y; vertices[3].z = 0;
	glPushMatrix();
	glMultMatrixf((float*)&modelToWorldMatrix);

	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	static Color color;
	color = Color::white() - Engine->GetClearColor();
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_QUADS);
	for (int i = 0; i < 4; ++i) glVertex3fv((float*)&vertices[i]);
	glEnd();
	glColor3f(color.r, color.g, color.b);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int i = 0; i < 4; ++i) DrawQuad(vertices[i], 0.03f * (rect.half_size_x + rect.half_size_y));

	glEnable(GL_DEPTH_TEST);

	glPopMatrix();
}