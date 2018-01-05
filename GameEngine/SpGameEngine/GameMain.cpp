#include"CApplication.h"

#include<gl\GL.h>
#include<gl\GLU.h>
#include"CInput.h"
#include"CDebug.h"
#include"CTime.h"
#include"CGameObject.h"
#include"CCamera.h"
#include"CEditorTool.h"
#include"CQuad.h"
#include"FileHead.h"

float x;
float y;
float z = 1;
float moveSpeed = 50;
Vector3 cameraPos;
CQuad* quad;
CQuad* quad2;

CPngFile* pngFile;
UINT texId;

C3DSFile* modelfile;

float scale = 0.1f;

int screenWidth = 1024;
int screenHight = 800;

const GLfloat flashLightPos[] = { -0.2f, -0.5f, 1, 1 };
const GLfloat flashLightColor[] = { 0.2f, 0.8f, 0.2f, 1.0f };
const GLfloat ambientLightColor[] = {0.1f, 0.1f, 0.1f, 1};

SApplicationInfo CApplication::GetApplicationInfo()
{
	return{ screenWidth, screenHight, 32, false, L"GameEngineTest", L"MainClass" };
}

void CApplication::OnStart()
{
	quad = new CQuad;
	quad->Translate(Vector3(0, 0, -5));

	pngFile = new CPngFile;
	pngFile->LoadFromFile("textures/model.png");
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, pngFile->GetInternalFormat(), pngFile->GetWidth(), pngFile->GetHeight(), 0, pngFile->GetFormat(), GL_UNSIGNED_BYTE, pngFile->GetData());
	glDisable(GL_TEXTURE_2D);

	modelfile = new C3DSFile;
	modelfile->LoadFromFile("models/model.3DS");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, flashLightPos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, flashLightColor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLightColor);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLightColor);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
}

void CApplication::OnUpdate()
{
	cameraPos.x = CInput::GetAxis("Horizontal") * CTime::deltaTime * moveSpeed;
	cameraPos.z = CInput::GetAxis("Vertical") * CTime::deltaTime * moveSpeed;

	Vector3 euler = CApplication::GetCamera()->GetEulerAngles();
	euler.y += CInput::GetAxis("MouseX") * CTime::deltaTime;
	euler.x += CInput::GetAxis("MouseY") * CTime::deltaTime;

	if (CInput::GetKey(DIK_UP))
	{
		scale += 0.01f;
		CDebug::Log("scale:%f", scale);
	}

	if (CInput::GetKey(DIK_DOWN))
	{
		scale -= 0.01f;
		CDebug::Log("scale:%f", scale);
	}

	//CApplication::GetCamera()->Rotate(euler);
	//CApplication::GetCamera()->Move(cameraPos.x, 0, cameraPos.z);
}

void CApplication::OnRender()
{
	CEditorTool::DrawGrid(CApplication::GetCamera()->GetPosition(), Vector3(0, 0, 0), Color(0, 0, 0.8, 1));

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, flashLightPos);

	glPushMatrix();
	glColor3f(0.5f, 0.5f, 0);
	
	glTranslatef(0, 1.5f, 5);
	glScalef(scale, scale, scale);
	glRotatef(CTime::time * 40.0f, 0, 1, 0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, (void*)modelfile->GetVertexArray());
	glNormalPointer(GL_FLOAT, 0, modelfile->GetNormalArray());
	glTexCoordPointer(2, GL_FLOAT, 0, modelfile->GetUVArray());
	glDrawArrays(GL_TRIANGLES, 0, modelfile->GetVertexNum());
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	//engine->BeginOrtho();
	//static int width = pngFile->GetWidth() >> 1;
	//static int height = pngFile->GetHeight() >> 1;
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, texId);
	//glTranslatef(CApplication::GetInstance()->GetWindowWidth() * 0.5f, CApplication::GetInstance()->GetWindowHeight() *0.5f, 0);
	//glBegin(GL_QUADS);
	//glTexCoord2f(1, 1);
	//glVertex3f(width, height, 0);
	//glTexCoord2f(0, 1);
	//glVertex3f(-width, height, 0);
	//glTexCoord2f(0, 0);
	//glVertex3f(-width, -height, 0);
	//glTexCoord2f(1, 0);
	//glVertex3f(width, -height, 0);
	//glEnd();
	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);
	//engine->EndOrtho();
}

void CApplication::OnClose()
{

}