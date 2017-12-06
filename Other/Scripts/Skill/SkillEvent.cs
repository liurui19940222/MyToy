using UnityEngine;

[System.Serializable]
public class SkillEvent {

    public ETriggerType TriggerType;             //该事件的触发类型
    public float DelayTime;                      //延迟时间
    public string AnimEvent;                     //动画事件
    public SkillCondition Condition;             //条件
    public bool BreakSkillWhenConditionFail;     //条件不满足时是否中断技能
    public ESkillActionType ActionType;          //操作类型
    public Object Params;                        //参数

    public ISkillAction CreateAction(Skill owner)
    {
        return Factory.Instance.CreateSkillAction(ActionType, owner, Params);
    }

    public enum ETriggerType
    {
        TimeLine,
        AnimEvent,
    }
}
