using UnityEngine;

[CreateAssetMenu(fileName = "equip_", menuName = "配置/装备")]
public class EquipmentConfig : ScriptableObject
{
    public int Id;                      //Id

    public string Name;                 //名字

    public string Desc;                 //描述

    public EEquipmentType Type;         //类型

    public string PrefabName;           //预设名字

    public int FirstSkill;              //起手技能(轻)

    public int FirstHeavySkill;         //起手技能(重)
}

