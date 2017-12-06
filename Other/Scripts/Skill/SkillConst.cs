public class SkillConst
{
    //技能条件描述
    public static string[] SkillConditionDesc =
    {
        "无",
        "AP大于某值",
        "HP小于某值",
        "HP大于某值",
        "MP小于某值",
        "MP大于某值",
        "仅在某技能之后",
        "仅在装备了某道具",
        "仅在装备了某武器",
        "仅在装备了某衣着(盔甲)",
        "仅在跳跃过程中",
        "仅在水中",
        "仅在火中",
        "仅在沼泽中",
    };

    //技能操作类型描述
    public static string[] SkillActionTypeDesc =
    {
        "播放骨骼动画",
        "播放特效",
        "播放音效",
        "伤害判定",
        "触发Buff",
        "触发另一个技能",
        "显示(隐藏)Object",
        "可以触发下一个技能",
        "结束技能",
        "结束触发下一个技能",
        "球状碰撞物",
    };
}

//技能条件
public enum ESkillCondition
{
    None,                   //无
    AP_Greater_,            //AP大于某值
    HP_Less_,               //HP小于某值
    HP_Greater_,            //HP大于某值
    MP_Less_,               //MP小于某值
    MP_Greater_,            //MP大于某值
    BeAfterSkill_,          //仅在某技能之后
    BeEquipedItem_,         //仅在装备了某道具
    BeEquipedWeapon_,       //仅在装备了某武器
    BeEquipedClothes_,      //仅在装备了某衣着(盔甲)
    Jumping_,               //仅在跳跃过程中
    InTheWater_,            //仅在水中
    InTheFire_,             //仅在火中
    InTheMarsh_,            //仅在沼泽中
}

//技能操作类型
public enum ESkillActionType
{
    PlayAnim,               //播放骨骼动画
    PlayEffect,             //播放特效
    PlaySound,              //播放音效
    CheckHurt,              //伤害判定
    TriggerBuff,            //触发Buff
    TriggerSkill,           //触发另一个技能
    ShowObject,             //显示(隐藏)Object
    ReadyToNext,            //可以触发下一个技能
    End,                    //结束技能
    StopToNext,             //结束触发下一个技能
    Ball,                   //球状碰撞物
}

//技能效果类型
public enum ESkillEffectType
{
    Hurt,                   //伤害
    Health,                 //恢复
    Buff,                   //持续性作用
}

//绑定位置（特效等）
public enum EBindPos
{
    None,
    MagicCore,
}