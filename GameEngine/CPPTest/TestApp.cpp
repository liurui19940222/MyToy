#include "TestApp.h"

#include<iostream>

TestApp::TestApp(const wchar_t* className, const wchar_t* title, int width, int height)
	: GLAppBase(className, title, width, height) {}

void TestApp::OnUpdate(float deltaTime)
{
	m_Angle += deltaTime * 200;
}

void TestApp::OnRender()
{
	GLAppBase::OnRender();

	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glRotatef(m_Angle, 0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glEnd();
	glPopMatrix();
}