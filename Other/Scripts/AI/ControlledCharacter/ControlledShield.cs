using UnityEngine;

public class ControlledShield : ControlledState
{
    public ControlledShield(int id, ICharacter ch) : base(id, ch)
    {
    }

    public override void OnEnter()
    {
        base.OnEnter();
        m_Character.PlaySheildAnim(true);
        Debug.Log("shield enter");
    }

    public override int OnUpdate()
    {
        m_Player.UpdateCamera();
        m_Player.UpdateMove(false);
        if (!InputSystem.Instance.GetInput(EInputWord.LB))
        {
            Debug.Log("松开");
            m_Character.PlaySheildAnim(false);
            return AIConst.CONTROLLED_AI_STATE_IDLE;
        }
        return base.OnUpdate();
    }

    public override void OnExit()
    {
        base.OnExit();
        Debug.Log("shield exit");
    }
}

