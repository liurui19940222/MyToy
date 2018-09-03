#include"Input.h"

USING_NAMESPACE_ENGINE

EMouseMode Input::Mode = EMouseMode::Relative;
LPDIRECTINPUT8 Input::lpDI;
LPDIRECTINPUTDEVICE8 Input::lpDIKeyboard;
LPDIRECTINPUTDEVICE8 Input::lpDIMouse;
byte Input::keyboard[256];
byte Input::keyboardHold[256];
DIMOUSESTATE Input::mouseStateData;
DIMOUSESTATE Input::mouseStateDataHold;
bool Input::isMouseKeyDown[3];
bool Input::isMouseKeyUp[3];
bool Input::isKeyboardDown[256];
bool Input::isKeyboardUp[256];
RECT Input::windowRect{0,0,0,0};

void Input::Init(HINSTANCE instance, HWND hwnd)
{
	DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID *)&lpDI, NULL);
	lpDI->CreateDevice(GUID_SysKeyboard, &lpDIKeyboard, NULL);
	lpDIKeyboard->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	lpDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	lpDIKeyboard->Acquire();

	lpDI->CreateDevice(GUID_SysMouse, &lpDIMouse, NULL);
	lpDIMouse->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	lpDIMouse->SetDataFormat(&c_dfDIMouse);
	lpDIMouse->Acquire();
}

void Input::GetState(const RECT& rect)
{
	windowRect = rect;
	lpDIKeyboard->GetDeviceState(256, (LPVOID)keyboard);
	lpDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&mouseStateData);
	for (int i = 0; i < 256; i++)
	{
		if (!(keyboard[i] & 0x80) && keyboardHold[i] != 0)
			keyboardHold[i] = 2;

		isKeyboardDown[i] = GetKeyDownState(i);
		isKeyboardUp[i] = GetKeyUpState(i);
	}
	for (int i = 0; i < 4; i++)
	{
		if (!(mouseStateData.rgbButtons[i]) && mouseStateDataHold.rgbButtons[i] != 0)
			mouseStateDataHold.rgbButtons[i] = 2;
	}

	for (int i = 0; i < 3; i++)
	{
		isMouseKeyDown[i] = GetMouseDownState((EMouseKey)i);
		isMouseKeyUp[i] = GetMouseUpState((EMouseKey)i);
	}
}

bool Input::GetKey(byte key)
{
	if (keyboard[key] & 0x80)
	{
		keyboardHold[key] = 1;
		return true;
	}
	return false;
}

bool Input::GetKeyDown(byte key)
{
	return isKeyboardDown[key];
}

bool Input::GetKeyUp(byte key)
{
	return isKeyboardUp[key];
}

bool Input::GetKeyDownState(byte key)
{
	if ((keyboard[key] & 0x80) && keyboardHold[key] != 1)
	{
		keyboardHold[key] = 1;
		return true;
	}
	return false;
}

bool Input::GetKeyUpState(byte key)
{
	if ((keyboardHold[key] == 1 && !(keyboard[key] & 0x80)) || keyboardHold[key] == 2)
	{
		keyboardHold[key] = 0;
		return true;
	}
	return false;
}

bool Input::GetMouseDown(EMouseKey key)
{
	return isMouseKeyDown[(EMouseKey)key];
}

bool Input::GetMouseUp(EMouseKey key)
{
	return isMouseKeyUp[(EMouseKey)key];
}

bool Input::GetMouse(EMouseKey key)
{
	return mouseStateData.rgbButtons[key];
}

bool Input::GetMouseDownState(EMouseKey key)
{
	if (mouseStateData.rgbButtons[(int)key] && !mouseStateDataHold.rgbButtons[(int)key])
	{
		mouseStateDataHold.rgbButtons[(int)key] = 1;
		return true;
	}
	return false;
}

bool Input::GetMouseUpState(EMouseKey key)
{
	if (mouseStateDataHold.rgbButtons[(int)key] && !mouseStateData.rgbButtons[(int)key] || mouseStateDataHold.rgbButtons[(int)key] == 2)
	{
		mouseStateDataHold.rgbButtons[(int)key] = 0;
		return true;
	}
	return false;
}

Vector2 Input::InputMousePosition(bool inverseY)
{
	POINT p;
	GetCursorPos(&p);
	if (Mode == EMouseMode::Relative)
	{
		int left = windowRect.left;
		int bottom = windowRect.bottom;
		if (inverseY)
			return Vector2(p.x - left, (bottom - windowRect.top) - (bottom - p.y));
		return Vector2(p.x - left, bottom - p.y);
	}
	return Vector2(p.x, p.y);
}

float Input::GetAxis(string axis)
{
	float value = 0;
	if (axis == "Horizontal")
	{
		if (GetKey(DIK_A) || GetKey(DIK_LEFT))
			value = -1;
		else if (GetKey(DIK_D) || GetKey(DIK_RIGHT))
			value = 1;
	}
	else if (axis == "Vertical")
	{
		if (GetKey(DIK_W) || GetKey(DIK_UP))
			value = 1;
		else if (GetKey(DIK_S) || GetKey(DIK_DOWN))
			value = -1;
	}
	else if (axis == "MouseX")
	{
		value = (float)mouseStateData.lX;
	}
	else if (axis == "MouseY")
	{
		value = (float)mouseStateData.lY;
	}
	else if (axis == "Scroll")
	{
		value = (float)mouseStateData.lZ;
	}
	return value;
}

void Input::ShutDown()
{
	if (lpDIKeyboard)
	{
		lpDIKeyboard->Unacquire();
		lpDIKeyboard->Release();
	}

	if (lpDIMouse)
	{
		lpDIMouse->Unacquire();
		lpDIMouse->Release();
	}

	if (lpDI)
		lpDI->Release();
}