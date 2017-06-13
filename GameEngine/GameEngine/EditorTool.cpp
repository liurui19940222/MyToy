#include "EditorTool.h"
#include<Windows.h>
#include<gl\GL.h>
#include<gl\GLU.h>

CEditorTool::CEditorTool()
{
}


CEditorTool::~CEditorTool()
{
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