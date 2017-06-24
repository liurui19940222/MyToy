#include "Input.h"
#include "Application.h"

LPDIRECTINPUT8 CInput::lpDI;
LPDIRECTINPUTDEVICE8 CInput::lpDIKeyboard;
LPDIRECTINPUTDEVICE8 CInput::lpDIMouse;
byte CInput::keyboard[256];
byte CInput::keyboardHold[256];
DIMOUSESTATE CInput::mouseStateData;
DIMOUSESTATE CInput::mouseStateDataHold;

void CInput::Init(HINSTANCE instance, HWND hwnd)
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

void CInput::GetState()
{
	lpDIKeyboard->GetDeviceState(256, (LPVOID)keyboard);
	lpDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&mouseStateData);
	for (int i = 0; i < 256; i++)
	{
		if (!(keyboard[i] & 0x80) && keyboardHold[i] != 0)
			keyboardHold[i] = 2;
	}
	for (int i = 0; i < 4; i++)
	{
		if (!(mouseStateData.rgbButtons[i]) && mouseStateDataHold.rgbButtons[i] != 0)
			mouseStateDataHold.rgbButtons[i] = 2;
	}
}

bool CInput::GetKey(byte key)
{
	if (keyboard[key] & 0x80)
	{
		keyboardHold[key] = 1;
		return true;
	}
	return false;
}

bool CInput::GetKeyDown(byte key)
{
	if ((keyboard[key] & 0x80) && keyboardHold[key] != 1)
	{
		keyboardHold[key] = 1;
		return true;
	}
	return false;
}

bool CInput::GetKeyUp(byte key)
{
	if ((keyboardHold[key] == 1 && !(keyboard[key] & 0x80)) || keyboardHold[key] == 2)
	{
		keyboardHold[key] = 0;
		return true;
	}
	return false;
}

bool CInput::GetMouseDown(EMouseKey key)
{
	if (mouseStateData.rgbButtons[(int)key] && !mouseStateDataHold.rgbButtons[(int)key])
	{
		mouseStateDataHold.rgbButtons[(int)key] = 1;
		return true;
	}
	return false;
}

bool CInput::GetMouseUp(EMouseKey key)
{
	if (mouseStateDataHold.rgbButtons[(int)key] && !mouseStateData.rgbButtons[(int)key] || mouseStateDataHold.rgbButtons[(int)key] == 2)
	{
		mouseStateDataHold.rgbButtons[(int)key] = 0;
		return true;
	}
	return false;
}

Vector2 CInput::InputMousePosition()
{
	POINT p;
	GetCursorPos(&p);
	int left = CApplication::GetInstance()->GetWindowRect()->left;
	int top = CApplication::GetInstance()->GetWindowRect()->top;
	return Vector2(p.x - CApplication::GetInstance()->GetWindowRect()->left, Application->GetWindowRect()->bottom - p.y);
}

float CInput::GetAxis(string axis)
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
		value = mouseStateData.lX;
	}
	else if (axis == "MouseY")
	{
		value = mouseStateData.lY;
	}
	else if (axis == "Scroll")
	{
		value = mouseStateData.lZ;
	}
	return value;
}

void CInput::ShutDown()
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