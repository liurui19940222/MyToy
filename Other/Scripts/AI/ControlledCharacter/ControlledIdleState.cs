using UnityEngine;
using DG.Tweening;

public class ControlledIdleState : ControlledState
{
    private HandleInputResult m_Result;

    public ControlledIdleState(int id, ICharacter ch) : base(id, ch)
    {

    }

    public override void OnEnter()
    {
        base.OnEnter();
        Debug.Log("idle enter");
    }

    public override int OnUpdate()
    {
        m_Player.UpdateCamera();
        if (m_Player.CanDoAction)
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
            //转到施放状态来处理攻击消息
            if (InputTranslater.IsAttackWord(word) && m_Player.CanDoAction)
            {
                m_Result = m_Player.GetInputResult(word, down);
                if (m_Result != null)
                {
                    if (m_Result.ResultType == HandleInputResult.Type.Sheild)
                    {
                        return AIConst.CONTROLLED_AI_STATE_SHIELD;
                    }
                    else if (m_Result.ResultType == HandleInputResult.Type.Skill)
                    {
                        IsHandledLastInput = false;
                        return AIConst.CONTROLLED_AI_STATE_RELEASE;
                    }
                }
            }
        }
        return base.HandleInput(input, msg);
    }

    public override void OnExit()
    {
        base.OnExit();
        Debug.Log("idle exit");
    }
}

