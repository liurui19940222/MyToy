/// <summary>
/// 对身体各部位的抽象
/// </summary>
public class IPartOfBody
{
    protected BattleCharacter m_Character;    //拥有者

    protected IEquipment m_Euipment;          //该部位的装备

    public IEquipment Euipment
    {
        get { return m_Euipment; }
        set { m_Euipment = value; }
    }

    public BattleCharacter Character
    {
        get { return m_Character; }
        set { m_Character = value; }
    }

    public virtual HandleInputResult HandleInput(EInputWord word, bool down)
    {
        return null;
    }
}

public class HandleInputResult
{
    public Type ResultType;

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
}


