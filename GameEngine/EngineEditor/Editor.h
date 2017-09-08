#ifndef _EDITOR_H_
#define _EDITOR_H_

#include<GameEngine\Engine.h>
#include<GameEngine\Singleton.h>
#include<windowsx.h>
#include<vector>

#define CLASS_NAME L"MainClass"
#define _Editor CEditor::GetInstance()

using namespace std;

class CChannel;

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

class CEditor : public CSingleton<CEditor>
{
private:
	HINSTANCE m_hInstance;
	HWND m_hwnd;
	float m_windowWidth;
	float m_windowHeight;

	vector<CChannel*> m_channels;

public:

	int EditorLoop();
	int InitEditor(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif