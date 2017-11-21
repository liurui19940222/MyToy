using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "skill_", menuName = "配置/技能")]
public class SkillConfig : ScriptableObject
{
    public int Id;                                  //技能Id

    public string Name;                             //技能名字

    public string Desc;                             //技能描述

    public bool ToStiff = true;                     //是否置释放者为僵直状态

    public List<SkillCondition> Conditions;         //释放前置条件

    public List<SkillTransition> Transitions;       //技能的跳转(该技能衔接下一个技能)，在ReadyToNext状态的时候判断是否跳转
        
    public List<SkillEvent> Events;                 //技能事件
}

[System.Serializable]
public class SkillCondition
{
    public ESkillCondition Condition;
    public string Param;

    public SkillCondition(ESkillCondition condition) { Condition = condition; }
}

[System.Serializable]
public class SkillTransition
{
    public EInputWord Input;                    //输入
    public int TargetSkillId;                   //输出
}
