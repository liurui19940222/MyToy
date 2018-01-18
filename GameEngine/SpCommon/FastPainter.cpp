#include "FastPainter.h"

HDC FastPainter::hdc;
HGLRC FastPainter::hrc;

void FastPainter::Init(HWND hwnd, int width, int height)
{
	hdc = GetDC(hwnd);
	SetupPixelFormat(hdc);
	hrc = wglCreateContext(hdc);
	MakeRenderContext();
	glewExperimental = GL_TRUE;
	glewInit();
}

void FastPainter::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
}

void FastPainter::Perspective(float fov, float aspect, float znear, float zfar)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, znear, zfar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void FastPainter::Viewport(int x, int y, int width, int height)
{
	glViewport(0, 0, width, height);
}

void FastPainter::MakeRenderContext()
{
	wglMakeCurrent(hdc, hrc);
}

void FastPainter::SetupPixelFormat(HDC hDC)
{
	int pixelFormat;
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);
}

void FastPainter::DrawQuad(const Vector3& position, const Color& color, float size)
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	float halfSize = size * 0.5f;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_QUADS);
	glVertex3f(-halfSize, halfSize, 0);
	glVertex3f(-halfSize, -halfSize, 0);
	glVertex3f(halfSize, -halfSize, 0);
	glVertex3f(halfSize, halfSize, 0);
	glEnd();
	glPopMatrix();
}

void FastPainter::DrawGrid(const Vector3& cameraPos, const Vector3& pos, const Color& color)
{
	static float cellSize = 1.0f;
	static float cellCount = 70.0f;
	static float length = cellSize * cellCount;
	static float cellLen = 0;

	//glEnable(GL_FOG);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glFogf(GL_FOG_DENSITY, 0.03f);
	glLineWidth(0.01f);
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

void FastPainter::DrawCone(const Matrix4x4& matrix, const Color& color, float radius, float height)
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

void FastPainter::DrawRect(const SRect2D& rect, const Matrix4x4& modelToWorldMatrix)
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
	color = Color::white;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_QUADS);
	for (int i = 0; i < 4; ++i) glVertex3fv((float*)&vertices[i]);
	glEnd();
	glColor3f(color.r, color.g, color.b);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int i = 0; i < 4; ++i) DrawQuad(vertices[i], Color::white, 0.03f * (rect.half_size_x + rect.half_size_y));

	glEnable(GL_DEPTH_TEST);

	glPopMatrix();
}

void FastPainter::DrawVector(const Vector3& vector, const Vector3& pos, const Color& color)
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

void FastPainter::DrawAxis(const Vector3& forward, const Vector3& right, const Vector3& up, const Vector3& pos)
{
	DrawVector(forward, pos, Color::blue);
	DrawVector(right, pos, Color::red);
	DrawVector(up, pos, Color::green);
}

void FastPainter::DrawAxis(Matrix4x4& modelToWorldMatrix, const Vector3& scale)
{
	static Vector3 forward, right, up, position;
	Matrix4x4::GetUVN(modelToWorldMatrix, scale, &right, &up, &forward);
	Matrix4x4::GetPosition(modelToWorldMatrix, &position);
	DrawAxis(forward, right, up, position);
}

void FastPainter::DrawAxis(Matrix4x4& modelToWorldMatrix)
{
	DrawAxis(modelToWorldMatrix, Vector3::one);
}
