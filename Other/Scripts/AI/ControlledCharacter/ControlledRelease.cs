using UnityEngine;

public class ControlledRelease : ControlledState
{
    private Skill m_CurSkill;               //当前施放的技能
    private HandleInputResult m_Result;     //手处理结果

    public ControlledRelease(int id, BattleCharacter ch) : base(id, ch)
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
            if (InputTranslater.IsAttackWord(word) && ReadyToExecute())
            {
                int skillId = 0;
                int lastSkillId = 0;
                //如果有正在施放的技能，取对应的衔接技能
                if (m_CurSkill != null)
                {
                    skillId = m_CurSkill.GetTargetId(word);
                    lastSkillId = m_CurSkill.Id;
                }
                if (skillId == 0)
                {
                    //根据手上的装备决定操作
                    m_Result = m_Player.GetInputResult(word, down);
                    if (m_Result != null)
                    {
                        if (m_Result.ResultType == HandleInputResult.Type.Skill)
                        {
                            skillId = m_Result.TargetSkillId;
                        }
                    }
                }
                //屏蔽相同的技能
                if (skillId != 0 && lastSkillId != skillId)
                {
                    m_CurSkill = RPGGame.Instance.ExecuteSkill(m_Player, skillId);
                    Debug.Log("execute skill " + m_CurSkill.Name);
                    return AIConst.CONTROLLED_AI_STATE_RELEASE;
                }
            }
        }
        return base.HandleInput(input, msg);
    }

    public override void OnExit()
    {
        base.OnExit();
        m_CurSkill = null;
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

