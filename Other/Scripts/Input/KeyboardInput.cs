using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class KeyboardInput : IInputDevice
{
    public override Vector2 GetAxis(EJoystick joystick)
    {
        if (joystick == EJoystick.LEFT)
        {
            m_Axis.x = Input.GetAxis("Horizontal");
            m_Axis.y = Input.GetAxis("Vertical");
        }
        else if (joystick == EJoystick.RIGHT)
        {
            m_Axis.x = Input.GetAxis("Mouse X");
            m_Axis.y = Input.GetAxis("Mouse Y");
        }
        return m_Axis;
    }

    public override bool GetInput(EInputWord input)
    {
        return false;
    }

    public override void UpdateInput()
    {
        KeyTest(KeyCode.UpArrow, EInputWord.DPAD_UP);
        KeyTest(KeyCode.DownArrow, EInputWord.DPAD_DOWN);
        KeyTest(KeyCode.LeftArrow, EInputWord.DPAD_LEFT);
        KeyTest(KeyCode.RightArrow, EInputWord.DPAD_RIGHT);
        KeyTest(KeyCode.Escape, EInputWord.BACK);
        KeyTest(KeyCode.Return, EInputWord.START);
        KeyTest(KeyCode.LeftShift, EInputWord.A);
        MouseTest(0, EInputWord.RB);
        MouseTest(1, EInputWord.RT);
    }

    private void KeyTest(KeyCode keyCode, EInputWord word)
    {
        if (Input.GetKeyDown(keyCode))
            m_InputSystem.OnInput(word, true);
        if (Input.GetKeyUp(keyCode))
            m_InputSystem.OnInput(word, false);
    }

    private void MouseTest(int mouseKey, EInputWord word)
    {
        if (Input.GetMouseButtonDown(mouseKey))
            m_InputSystem.OnInput(word, true);
        if (Input.GetMouseButtonUp(mouseKey))
            m_InputSystem.OnInput(word, false);
    }
}
