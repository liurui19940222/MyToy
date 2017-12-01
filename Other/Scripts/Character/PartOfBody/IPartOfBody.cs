using UnityEngine;

/// <summary>
/// 对身体各部位的抽象
/// </summary>
public class IPartOfBody
{
    protected ICharacter m_Character;    //拥有者

    protected IEquipment m_Euipment;     //该部位的装备

    private Transform transform;

    public virtual IEquipment Euipment
    {
        get { return m_Euipment; }
        set { m_Euipment = value; }
    }

    public ICharacter Character
    {
        get { return m_Character; }
        set { m_Character = value; }
    }

    public Transform Transform
    {
        get { return transform; }
        set { transform = value; }
    }

    public virtual HandleInputResult HandleInput(EInputWord word, bool down)
    {
        return null;
    }
}

public class HandleInputResult
{
    public Type ResultType;

    public EPartOfBodyType ByWitchHand; //被哪只手处理的

    public int TargetSkillId;

    public enum Type
    {
        Sheild,
        Skill,
    }
}

/// <summary>
/// 身体各部位的类型
/// </summary>
public enum EPartOfBodyType
{
    Head,                   //头
    Chest,                  //胸腔
    Back,                   //后背
    Shoulders,              //左肩
    Arms,                   //手臂
    Waist,                  //腰
    LeftHand,               //左手
    RightHand,              //右手
    Legs,                   //腿
    Foots,                  //脚
    None,
}


