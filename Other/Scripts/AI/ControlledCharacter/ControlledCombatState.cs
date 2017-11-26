using UnityEngine;
using DG.Tweening;

public class ControlledCombatState : ControlledState
{
    private HandleInputResult m_Result;
    private float m_Timer;
    private float m_PrepareTime = 0.0f;
    private bool m_PutWeaponBack = false;
    private ICharacter m_LockTarget;

    public ControlledCombatState(int id, ICharacter ch) : base(id, ch)
    {

    }

    public override void OnEnter()
    {
        base.OnEnter();
        Debug.Log("combat enter");
        m_Player.PlayCombatAnim(true);
        m_Timer = 0;
        m_PutWeaponBack = false;
    }

    public override int OnUpdate()
    {
        m_Timer += Time.deltaTime;
        if (m_Timer > m_PrepareTime)
        {
            if (m_PutWeaponBack)
            {
                //return AIConst.CONTROLLED_AI_STATE_RELAX;
            }
            else
            {
                if (m_Player.CanDoAction)
                {
                    m_Player.UpdateMove(true);
                }
            }
        }
        m_Player.UpdateCamera();
        return base.OnUpdate();
    }

    public override int HandleInput(EFSMInputType input, Message msg)
    {
        if (m_PutWeaponBack)
            return 0;
        EInputWord word = (EInputWord)msg["word"];
        bool down = (bool)msg["down"];
        if (down)
        {
            /*if (word == EInputWord.DPAD_RIGHT) //准备收起武器（因为有动作时间）
            {
                m_PutWeaponBack = true;
                m_Timer = 0;
                //顺便取消锁定
                m_LockTarget = null;
                m_Player.UnlockEnemy();
            }
            else */if (word == EInputWord.R3) //锁定一个敌人
            {
                m_Player.CheckLockEnemy();
            }
            else if (InputTranslater.IsAttackWord(word) && m_Player.CanDoAction)
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
        Debug.Log("combat exit");
        m_Player.PlayCombatAnim(false);
    }
}

