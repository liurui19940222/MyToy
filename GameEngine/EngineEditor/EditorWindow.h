#include<GameEngine\Application.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void HandleMenu(int menuId);

class CEditorWindow : public CGameWindow
{
	CGameObject* go;

public:
	virtual void OnStart();

	virtual void OnUpdate();

	virtual void GetApplicationInfo(SApplicationInfo* info);
};

