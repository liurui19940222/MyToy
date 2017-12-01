using UnityEngine;
using System.Collections.Generic;

public class SkillFactory
{
    public ISkillAction CreateAction(ESkillActionType action, Skill owner, Object _params)
    {
        ISkillAction skillAction = null;
        switch (action)
        {
            case ESkillActionType.PlayAnim:
                skillAction = new PlayAnimAction(owner, _params);
                break;
            case ESkillActionType.PlayEffect:
                skillAction = new PlayEffectAction(owner, _params);
                break;
            case ESkillActionType.PlaySound:
                skillAction = new PlaySoundAction(owner, _params);
                break;
            case ESkillActionType.CheckHurt:
                skillAction = new CheckHurtAction(owner, _params);
                break;
            case ESkillActionType.TriggerBuff:
                break;
            case ESkillActionType.TriggerSkill:
                break;
            case ESkillActionType.ShowObject:
                break;
            case ESkillActionType.ReadyToNext:
                skillAction = new ReadyToNextAction(owner, _params);
                break;
            case ESkillActionType.End:
                skillAction = new EndSkillAction(owner, _params);
                break;
            case ESkillActionType.StopToNext:
                skillAction = new StopToNextAction(owner, _params);
                break;
            default:
                break;
        }
        return skillAction;
    }
}

