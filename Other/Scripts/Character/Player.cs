using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : BattleCharacter
{
    public override void OnInitialize()
    {
        base.OnInitialize();
    }

    //更新相机的转向
    public void UpdateCamera()
    {
        if (m_GameCamera != null)
        {
            m_GameCamera.HandleAxis(InputSystem.Instance.GetAxis(EJoystick.RIGHT));
        }
    }

    //更新移动的输入
    public void UpdateMove(bool handleRun)
    {
        Vector3 dir = InputSystem.Instance.GetAxis(EJoystick.LEFT);
        dir.z = dir.y;
        dir.y = 0;
        if (handleRun && InputSystem.Instance.GetInput(EInputWord.A))
            dir *= 4;
        Move(dir);
    }

    public override void SetToIdleState()
    {
        base.SetToIdleState();
        m_FSM.SwitchToState(AIConst.CONTROLLED_AI_STATE_IDLE);
    }
}
