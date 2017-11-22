using UnityEngine;

[CreateAssetMenu(fileName = "equip_", menuName = "配置/装备")]
public class EquipmentConfig : ScriptableObject
{
    public int Id;

    public EEquipmentType Type;

    public string PrefabName;
}

