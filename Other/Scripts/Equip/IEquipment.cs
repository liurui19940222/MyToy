public class IEquipment
{
    private EquipmentConfig m_Config;

    public EEquipmentType Type { get { return m_Config.Type; } }

    public EEquipmentMateril Material { get { return m_Config.Material; } }

    public int FirstSkill { get { return m_Config.FirstSkill; } }

    public int FirstHeavySkill { get { return m_Config.FirstHeavySkill; } }

    public IEquipment(int id)
    {
        m_Config = ResourceFactory.Instance.LoadEquipConfig(id);
    }

    //是否是利器
    public bool IsSharpWeapon()
    {
        return Type == EEquipmentType.Rapier || Type == EEquipmentType.Sword || 
            Type == EEquipmentType.DoubleSword || Type == EEquipmentType.GreatSword || 
            Type == EEquipmentType.Dagger || Type == EEquipmentType.Axe;
    }

    //是否是钝器
    public bool IsBluntWeapon()
    {
        return Type == EEquipmentType.Stick || Type == EEquipmentType.MagicStick;
    }
}

public enum EEquipmentMateril
{
    Wood,
    Iron,
}

public enum EEquipmentType
{
    Hat,                    //帽子(头盔)
    Clothes,                //衣服(盔甲)
    Armguards,              //臂甲
    Trousers,               //裤子
    Belt,                   //腰带(腰甲)
    Shoes,                  //鞋子
    Shield,                 //盾
    Rapier,                 //西洋剑
    Sword,                  //长剑
    DoubleSword,            //双剑
    GreatSword,             //巨剑
    Dagger,                 //匕首
    Axe,                    //斧头(长柄斧)
    Stick,                  //棍子
    MagicStick,             //魔杖
}

