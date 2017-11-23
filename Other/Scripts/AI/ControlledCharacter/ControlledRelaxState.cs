using UnityEngine;
using DG.Tweening;

public class ControlledRelaxState : ControlledState
{
    private HandleInputResult m_Result;

    public ControlledRelaxState(int id, BattleCharacter ch) : base(id, ch)
    {

    }

    public override void OnEnter()
    {
        base.OnEnter();
        Debug.Log("relax enter");
    }

    public override int OnUpdate()
    {
        m_Player.UpdateCamera();
        if (!m_Player.Stiff)
        {
            m_Player.UpdateMove(true);
        }
        return base.OnUpdate();
    }

    public override int HandleInput(EFSMInputType input, Message msg)
    {
        EInputWord word = (EInputWord)msg["word"];
        bool down = (bool)msg["down"];
        if (down)
        {
            if (word == EInputWord.DPAD_LEFT || word == EInputWord.DPAD_RIGHT)
            {
                IsHandledLastInput = true;
                return AIConst.CONTROLLED_AI_STATE_COMBAT;
            }
        }
        return base.HandleInput(input, msg);
    }

    public override void OnExit()
    {
        base.OnExit();
        Debug.Log("relax exit");
    }
}

