#include<Windows.h>
#include<gl\glew.h>
#include"EditorTool.h"
#include"Engine.h"
#include"Application.h"
#include"Debug.h"
#include"GameObject.h"
#include"Input.h"
#include"Time.h"
#include"EngineSetting.h"

void CEditorTool::DrawQuad(const Vector3& position, float size)
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

void CEditorTool::DrawGrid(const Vector3& cameraPos, const Vector3& pos, const Color& color)
{
	static float cellSize = 1.0f;
	static float cellCount = 70.0f;
	static float length = cellSize * cellCount;
	static float cellLen = 0;

	//glEnable(GL_FOG);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glFogf(GL_FOG_DENSITY, 0.03f);
	glLineWidth(CEngineSetting::GridStrength);
	glPushMatrix();
	glColor3f(color.r, color.g, color.b);
	glTranslatef(pos.x - length * 0.5f + cameraPos.x, pos.y, pos.z + length * 0.5f + cameraPos.z);
	glBegin(GL_LINES);
	for (int i = 0; i <= cellCount; i++)
	{
		cellLen = i * cellSize;
		glVertex3f(cellLen, 0, 0);
		glVertex3f(cellLen, 0, -length);
		glVertex3f(0, 0, -cellLen);
		glVertex3f(length, 0, -cellLen);
	}
	glEnd();
	glPopMatrix();
	glLineWidth(1);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
}

void CEditorTool::DrawCone(const Matrix4x4& matrix, const Color& color, float radius, float height)
{
	const int VERT_COUNT = 18;
	float rad = CMath::DegToRad * ((float)360 / (float)VERT_COUNT);
	float vertex_y = height * 0.5f;
	Vector3 circle[VERT_COUNT];

	for (int i = 0; i < VERT_COUNT; ++i)
	{
		float x = cos(rad * i) * radius;
		float y = sin(rad * i) * radius;
		circle[i].z = -vertex_y;
		circle[i].x = x;
		circle[i].y = y;
	}
	glColor3f(color.r, color.g, color.b);
	glPushMatrix();
	glMultMatrixf((float*)&matrix);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, -vertex_y);
	for (int i = 0; i < VERT_COUNT; ++i)
	{
		glVertex3f(circle[i].x, circle[i].y, circle[i].z);
	}
	glVertex3f(circle[0].x, circle[0].y, circle[0].z);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, vertex_y);
	for (int i = 0; i < VERT_COUNT; ++i)
	{
		glVertex3f(circle[i].x, circle[i].y, circle[i].z);
	}
	glVertex3f(circle[0].x, circle[0].y, circle[0].z);
	glEnd();
	glPopMatrix();
}

void CEditorTool::DrawRect(const SRect2D& rect, const Matrix4x4& modelToWorldMatrix)
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
	if (_MainCamera)
		color = Color::white - _MainCamera->GetCameraClearColor();
	else
		color = Color::white;
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
	//glDisable(GL_DEPTH_TEST);
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
	//glEnable(GL_DEPTH_TEST);
}

void CEditorTool::DrawAxis(const Vector3& forward, const Vector3& right, const Vector3& up, const Vector3& pos)
{
	DrawVector(forward, pos, Color::blue);
	DrawVector(right, pos, Color::red);
	DrawVector(up, pos, Color::green);
}

void CEditorTool::DrawAxis(Matrix4x4& modelToWorldMatrix, const Vector3& scale)
{
	static Vector3 forward, right, up, position;
	Matrix4x4::GetUVN(modelToWorldMatrix, scale, &right, &up, &forward);
	Matrix4x4::GetPosition(modelToWorldMatrix, &position);
	DrawAxis(forward, right, up, position);
}

void CEditorTool::DrawAxis(Matrix4x4& modelToWorldMatrix)
{
	DrawAxis(modelToWorldMatrix, Vector3::one);
}

struct JointVertex
{
	Matrix4x4 m_matrix;
	Vector3 m_pos;
	byte m_parent = 0xFF;
};

void CEditorTool::DrawSkeleton(Matrix4x4& modelToWorldMatrix, Skeleton& skeleton)
{
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glPointSize(3);
	//glScalef(2.5, 2.5, 2.5);
	glMultMatrixf((float*)&(modelToWorldMatrix));
	glColor3f(1, 0, 0);
	JointVertex* vertices = (JointVertex*)malloc(sizeof(JointVertex) * skeleton.GetSize());
	int index = 0;
	for (Joint& joint : skeleton.GetJoints())
	{
		JointVertex v;
		v.m_matrix = skeleton.m_globalPoses[joint.m_Index];
		v.m_pos = skeleton.m_globalPoses[joint.m_Index] * Vector4(0, 0, 0, 1);
		v.m_parent = joint.m_iParent;
		vertices[index++] = v;
	}
	
	glBegin(GL_POINTS);
	for (int i = 0; i < skeleton.GetSize() - 1; i++)
	{
		JointVertex vertex = vertices[i];
		glVertex3f(vertex.m_pos.x, vertex.m_pos.y, vertex.m_pos.z);
	}
	glEnd();

	for (int i = 0; i < skeleton.GetSize() - 1; i++)
	{
		JointVertex vertex = vertices[i];
		if (vertex.m_parent != 0xFF)
		{
			JointVertex parent = vertices[vertex.m_parent];
			Vector3 position = (vertex.m_pos - parent.m_pos) * 0.5f + parent.m_pos;
			float length = (vertex.m_pos - parent.m_pos).Magnitude();
			CEditorTool::DrawCone(Matrix4x4::Translate(position) * Matrix4x4::RotateUVN(vertex.m_pos, parent.m_pos), Color::Hex(0x008B00FF), 0.08 * length, length);
		}
	}

	glColor3f(0.8, 1, 0);
	glBegin(GL_LINES);
	for (int i = 0; i < skeleton.GetSize() - 1; i++)
	{
		JointVertex vertex = vertices[i];
		if (vertex.m_parent != 0xFF)
		{
			JointVertex parent = vertices[vertex.m_parent]; 
			glVertex3f(vertex.m_pos.x, vertex.m_pos.y, vertex.m_pos.z);
			glVertex3f(parent.m_pos.x, parent.m_pos.y, parent.m_pos.z);
		}
	}
	glEnd();
	glPopMatrix();
	free(vertices);
	glEnable(GL_DEPTH_TEST);
}

void CEditorTool::PrintTree(bool showDepth)
{
	CDebug::Log("---------------the scene's tree---------------");
	_Maker->ForeachGameObject([showDepth](CGameObject* go, int depth) {
		PrintTree(go, depth, showDepth);
	});
	CDebug::Log("----------------------------------------------");
}

void CEditorTool::PrintTree(CGameObject* go, bool showDepth)
{
	CDebug::Log("---------------the scene's tree---------------");
	_Maker->ForeachGameObject(go, [showDepth](CGameObject* go, int depth) {
		PrintTree(go, depth, showDepth);
	});
	CDebug::Log("----------------------------------------------");
}

void CEditorTool::PrintTree(CGameObject* go, int depth, bool showDepth)
{
	wstring str;
	for (int i = 0; i < depth; i++)
	{
		str += L"   ";
	}
	if (showDepth)
		str = str + go->GetName() + L" " + (wchar_t)('0' + depth);
	else
		str = str + go->GetName();
	CDebug::Log(str.c_str());
}

void CEditorTool::WatchTarget(CGameObject& camera, const Vector3& targetPos)
{
	static Vector3 eyePos(0, 6.8, 10);
	static float speed = 50.0f;
	static bool key_down = false;
	static Vector2 lastMousePos(0.0f, 0.0f);
	static Vector2 curMousePos(0.0f, 0.0f);
	static float distance = (eyePos - targetPos).Magnitude();
	if (CInput::GetMouseDown(EMouseKey::Right))
	{
		key_down = true;
		lastMousePos = CInput::InputMousePosition();
		_Application->SetCursor(IDC_CROSS);
	}
	if (CInput::GetMouseUp(EMouseKey::Right))
	{
		key_down = false;
		_Application->SetCursor(IDC_ARROW);
	}
	distance -= CInput::GetAxis("Scroll") * CTime::deltaTime;
	distance = CMath::Clamp(distance, 8.0f, 20.0f);
	Vector3 dir = (camera.GetLocalPosition() - targetPos).Normalize() * distance;
	camera.SetLocalPosition(dir + targetPos);
	if (!key_down) return;
	curMousePos = CInput::InputMousePosition();
	float h = (curMousePos.x - lastMousePos.x) * CTime::deltaTime * speed;
	float v = (curMousePos.y - lastMousePos.y) * CTime::deltaTime * speed;
	lastMousePos = curMousePos;
	dir = Quaternion::AngleAxis(Vector3::up, -h) * Quaternion::AngleAxis(Vector3::Cross(dir, Vector3::up), -v) * dir;
	float angle = Vector3::Angle(dir, Vector3::up);
	if (angle  < 10.0f || angle > 170.0f) return;
	camera.SetLocalPosition(dir + targetPos);
	camera.LookAt(targetPos);
}