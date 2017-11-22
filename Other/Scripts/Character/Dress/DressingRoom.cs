using System;

/// <summary>
/// 实现具体的穿戴逻辑
/// </summary>
public class DressingRoom : IDressingRoom
{
    public override void Wear(BattleCharacter ch, IEquipment equip, EBodySide side)
    {
        switch (equip.Type)
        {
            case EEquipmentType.Hat:
                
                break;
            case EEquipmentType.Clothes:
                break;
            case EEquipmentType.Armguards:
                break;
            case EEquipmentType.Trousers:
                break;
            case EEquipmentType.Belt:
                break;
            case EEquipmentType.Shoes:
                break;
            case EEquipmentType.Rapier:
                break;
            case EEquipmentType.Sword:
                break;
            case EEquipmentType.DoubleSword:
                break;
            case EEquipmentType.GreatSword:
                break;
            case EEquipmentType.Dagger:
                break;
            case EEquipmentType.Axe:
                break;
            case EEquipmentType.Stick:
                break;
            case EEquipmentType.MagicStick:
                break;
            default:
                break;
        }
    }
}

