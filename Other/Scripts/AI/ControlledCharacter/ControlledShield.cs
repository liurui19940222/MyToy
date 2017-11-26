using UnityEngine;

public class ControlledShield : ControlledState
{
    public ControlledShield(int id, ICharacter ch) : base(id, ch)
    {
    }

    public override void OnEnter()
    {
        base.OnEnter();
        m_Character.Defend(true);
        Debug.Log("shield enter");
    }

    public override int OnUpdate()
    {
        m_Player.UpdateCamera();
        m_Player.UpdateMove(false);
        if (!InputSystem.Instance.GetInput(EInputWord.LB))
        {
            m_Character.Defend(false);
            return AIConst.CONTROLLED_AI_STATE_COMBAT;
        }
        return base.OnUpdate();
    }

    public override int HandleInput(EFSMInputType input, Message msg)
    {
        EInputWord word = (EInputWord)msg["word"];
        bool down = (bool)msg["down"];
        if (down)
        {
            if (word == EInputWord.R3) //锁定一个敌人
            {
                m_Player.CheckLockEnemy();
            }
        }
        return base.HandleInput(input, msg);
    }

    public override void OnExit()
    {
        base.OnExit();
        Debug.Log("shield exit");
    }
}

