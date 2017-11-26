using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : ICharacter
{
    public override void OnInitialize()
    {
        base.OnInitialize();
    }

    public override void OnUpdate()
    {
        base.OnUpdate();
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

    //锁定敌人
    public void LockEnemy(ICharacter ch)
    {
        m_GameCamera.LockEnemy(ch);
        SetAnimStateToLock(true);
        IsLocking = true;
        LockedEnemy = ch;
        RPGGame.Instance.ShowLockTargetHUD(ch.Transform, ch.CenterPosition);
    }

    //取消锁定敌人
    public void UnlockEnemy()
    {
        m_GameCamera.UnlockEnemy();
        SetAnimStateToLock(false);
        IsLocking = false;
        LockedEnemy = null;
        RPGGame.Instance.HideLockTargetHUD();
    }

    //锁定或取消锁定
    public void CheckLockEnemy()
    {
        if (LockedEnemy == null)
        {
            LockedEnemy = RPGGame.Instance.GetACharacterWithFanShape(this, this.Transform.forward, 120, 8);
            if (LockedEnemy != null)
            {
                LockEnemy(LockedEnemy);
            }
        }
        else
        {
            LockedEnemy = null;
            UnlockEnemy();
        }
    }

    public override void SetToIdleState()
    {
        base.SetToIdleState();
        m_FSM.SwitchToState(AIConst.CONTROLLED_AI_STATE_COMBAT);
    }
}
