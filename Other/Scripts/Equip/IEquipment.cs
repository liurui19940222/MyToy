using UnityEngine;

public class IEquipment
{
    private EquipmentConfig m_Config;

    public EEquipmentType Type { get { return m_Config.Type; } }

    public EEquipmentMateril Material { get { return m_Config.Material; } }

    public int FirstSkill { get { return m_Config.FirstSkill; } }

    public int FirstHeavySkill { get { return m_Config.FirstHeavySkill; } }

    private Transform m_Transform;
    private GameObject m_GameObject;

    public Transform Transform
    {
        get { return m_Transform; }
        set { m_Transform = value; }
    }

    public GameObject GameObject
    {
        get { return m_GameObject; }
        set { m_GameObject = value; }
    }

    public IEquipment(EquipmentConfig config)
    {
        m_Config = config;
        if (!string.IsNullOrEmpty(m_Config.PrefabName))
        {
            m_GameObject = GameObject.Instantiate(ResourceFactory.Instance.LoadEquipment(m_Config.PrefabName)) as GameObject;
            m_Transform = m_GameObject.transform;
        }
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

    //是否是武器
    public static bool IsWeapon(EEquipmentType type)
    {
        switch (type)
        {
            case EEquipmentType.Hat:
            case EEquipmentType.Clothes:
            case EEquipmentType.Armguards:
            case EEquipmentType.Trousers:
            case EEquipmentType.Belt:
            case EEquipmentType.Shoes:
            case EEquipmentType.Shield:
                return false;
        }
        return true;
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

