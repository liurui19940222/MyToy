#ifndef _CINPUT_
#define _CINPUT_

#define INITGUID

#include<Windows.h>
#include<windowsx.h>
#include<string>
#include<dinput.h>
#include"..\SpCommon\Math.h"
#include"..\SpCommon\ApiDefine.h"

using namespace std;

enum ENGINE_API EMouseKey : int
{
	Left = 0,
	Right = 1,
	Center = 2
};

enum ENGINE_API EMouseMode : int
{
	Absolute,
	Relative,
};

class ENGINE_API CInput
{
private:
	static LPDIRECTINPUT8 lpDI;
	static LPDIRECTINPUTDEVICE8 lpDIKeyboard;
	static LPDIRECTINPUTDEVICE8 lpDIMouse;
	static byte keyboard[256];
	static byte keyboardHold[256];
	static DIMOUSESTATE mouseStateData;
	static DIMOUSESTATE mouseStateDataHold;
	static bool isMouseKeyDown[3];
	static bool isMouseKeyUp[3];
	static bool isKeyboardDown[256];
	static bool isKeyboardUp[256];

	static bool GetMouseDownState(EMouseKey key);
	static bool GetMouseUpState(EMouseKey key);
	static bool GetKeyDownState(byte key);
	static bool GetKeyUpState(byte key);
public:
	static EMouseMode Mode;
	static void Init(HINSTANCE instance, HWND hwnd);
	static void GetState();
	static bool GetKey(byte key);
	static bool GetKeyDown(byte key);
	static bool GetKeyUp(byte key);
	static bool GetMouseDown(EMouseKey key);
	static bool GetMouseUp(EMouseKey key);
	static bool GetMouse(EMouseKey key);
	static Vector2 InputMousePosition();
	static float GetAxis(string axis);
	static void ShutDown();
};

#endif