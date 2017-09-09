#ifndef _EDITOR_H_
#define _EDITOR_H_

#include<GameEngine\Engine.h>
#include<GameEngine\Singleton.h>
#include<GameEngine\Property.h>
#include<windowsx.h>
#include<map>

#define CLASS_NAME L"MainClass"
#define _Editor CEditor::GetInstance()

using namespace std;

class CWindow;

enum EWindowType
{
	Other,
	Main,
	Scene,
	WorldTree,
	Console,
	Watcher,
};

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

class CEditor : public CSingleton<CEditor>
{
private:
	HINSTANCE m_hInstance;
	float m_windowWidth;
	float m_windowHeight;

	map<EWindowType, CWindow*> m_windows;

public:

	void UpdateSize();
	int EditorLoop();
	int InitEditor(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
	vector<CWindow*> GetWindowsExcept(EWindowType type);

	template<typename T> T* OpenWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD style)
	{
		CWindow* window = new T(class_name, instance, parent, width, height, style);
		if (window->GetType() == EWindowType::Other)
			return dynamic_cast<T*>(window);
		m_windows.insert(make_pair(window->GetType(), window));
		return dynamic_cast<T*>(window);
	}

	template<typename T> void CloseWindow(T* t)
	{
		CWindow* window = (CWindow*)t;
		auto it = m_windows.find(window->GetType());
		if (it != m_windows.end())
		{
			m_windows.erase(it);
		}
		delete(t);
	}

	property_r<HINSTANCE> WinInstance = _prop_r(HINSTANCE, { return m_hInstance; });
};

#endif