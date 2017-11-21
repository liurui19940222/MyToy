using UnityEngine;

[System.Serializable]
public class SkillEvent {

    public float DelayTime;                      //延迟时间
    public SkillCondition Condition;            //条件
    public bool BreakSkillWhenConditionFail;     //条件不满足时是否中断技能
    public ESkillActionType ActionType;          //操作类型
    public Object Params;                        //参数

    public ISkillAction CreateAction(Skill owner)
    {
        return Factory.Instance.CreateSkillAction(ActionType, owner, Params);
    }
}
