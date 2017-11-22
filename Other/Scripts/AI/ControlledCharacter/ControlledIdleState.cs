using UnityEngine;
using DG.Tweening;

public class ControlledIdleState : ControlledState
{
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
        if (!m_Player.Stiff)
        {
            m_Player.UpdateMove(true);
            if (InputSystem.Instance.GetInput(EInputWord.LB))
            {
                return AIConst.CONTROLLED_AI_STATE_SHIELD;
            }
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
            if (InputTranslater.IsAttackWord(word) && !m_Player.Stiff)
            {
                IsHandledLastInput = false;
                return AIConst.CONTROLLED_AI_STATE_RELEASE;
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

