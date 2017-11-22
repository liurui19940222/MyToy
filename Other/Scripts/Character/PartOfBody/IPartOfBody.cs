/// <summary>
/// 对身体各部位的抽象
/// </summary>
public class IPartOfBody
{
    private IEquipment m_Euipment;          //该部位的装备

    public IEquipment Euipment
    {
        get { return m_Euipment; }
        set { m_Euipment = value; }
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
    LeftShoulder,           //左肩
    RightShoulder,          //右肩
    LeftArm,                //左臂
    RightArm,               //右臂
    Waist,                  //腰
    LeftHand,               //左手
    RightHand,              //右手
    LeftLeg,                //左腿
    RightLeg,               //右腿
    LeftFoot,               //左脚
    RightFoot,              //右脚
}


