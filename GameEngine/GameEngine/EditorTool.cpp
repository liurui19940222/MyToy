#include<Windows.h>
#include<gl\GL.h>
#include<gl\GLU.h>
#include"EditorTool.h"
#include"Engine.h"
#include"Application.h"
#include"Debug.h"

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

void CEditorTool::DrawVector(const Vector3& vector, const Vector3& pos, const Color& color)
{
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	float width;
	glGetFloatv(GL_LINE_WIDTH, &width);
	glLineWidth(3);
	glTranslatef(pos.x, pos.y, pos.z);
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(vector.x, vector.y, vector.z);
	glEnd();
	glLineWidth(width);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void CEditorTool::DrawAxis(const Vector3& forward, const Vector3& right, const Vector3& up, const Vector3& pos)
{
	DrawVector(forward, pos, Color::blue());
	DrawVector(right, pos, Color::red());
	DrawVector(up, pos, Color::green());
}

void CEditorTool::DrawAxis(const CGameObject* go)
{
	DrawAxis(go->GetForward(), go->GetRight(), go->GetUp(), go->GetPosition());
}

void CEditorTool::PrintTree(bool showDepth)
{
	Engine->ForeachGameObject([showDepth](CGameObject* go, int depth) {
		PrintTree(go, depth, showDepth);
	});
}

void CEditorTool::PrintTree(CGameObject* go, bool showDepth)
{
	Engine->ForeachGameObject(go, [showDepth](CGameObject* go, int depth) {
		PrintTree(go, depth, showDepth);
	});
}

void CEditorTool::PrintTree(CGameObject* go, int depth, bool showDepth)
{
	string str;
	for (int i = 0; i < depth; i++)
	{
		str += "   ";
	}
	if (showDepth)
		str = str + go->GetName() + " " + (char)('0' + depth);
	else
		str = str + go->GetName();
	CDebug::Log(str.c_str());
}