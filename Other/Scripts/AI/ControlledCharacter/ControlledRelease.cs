using UnityEngine;

public class ControlledRelease : ControlledState
{
    private Skill m_CurSkill;   //当前施放的技能

    public ControlledRelease(int id, ICharacter ch) : base(id, ch)
    {
    }

    public override void OnEnter()
    {
        base.OnEnter();
        Debug.Log("release enter");
    }

    public override int OnUpdate()
    {
        m_Player.UpdateCamera();
        return base.OnUpdate();
    }

    public override int HandleInput(EFSMInputType input, Message msg)
    {
        EInputWord word = (EInputWord)msg["word"];
        bool down = (bool)msg["down"];
        if (down)
        {
            if (word == EInputWord.RB && ReadyToExecute())
            {
                Debug.Log("execute skill");
                m_CurSkill = RPGGame.Instance.ExecuteSkill(m_Player, 1);
                return AIConst.CONTROLLED_AI_STATE_RELEASE;
            }
        }
        return base.HandleInput(input, msg);
    }

    public override void OnExit()
    {
        base.OnExit();
        Debug.Log("release exit");
    }

    //是否准备好施放技能
    private bool ReadyToExecute()
    {
        if (!m_Player.Stiff)
            return true;
        if (m_CurSkill != null && m_CurSkill.ReadyToNext)
            return true;
        return false;
    }
}

