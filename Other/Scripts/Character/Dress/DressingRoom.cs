using System;
using UnityEngine;

/// <summary>
/// 实现具体的穿戴逻辑
/// </summary>
public class DressingRoom : IDressingRoom
{
    public override void Wear(BattleCharacter ch, IEquipment equipment, EBodySide side)
    {
        switch (equipment.Type)
        {
            case EEquipmentType.Hat:
                ch.Wear(EPartOfBodyType.Head, equipment);
                break;
            case EEquipmentType.Clothes:
                ch.Wear(EPartOfBodyType.Chest, equipment);
                break;
            case EEquipmentType.Armguards:
                ch.Wear(EPartOfBodyType.Arms, equipment);
                break;
            case EEquipmentType.Trousers:
                ch.Wear(EPartOfBodyType.Legs, equipment);
                break;
            case EEquipmentType.Belt:
                ch.Wear(EPartOfBodyType.Waist, equipment);
                break;
            case EEquipmentType.Shoes:
                ch.Wear(EPartOfBodyType.Foots, equipment);
                break;
            case EEquipmentType.Shield:
            case EEquipmentType.Rapier:
            case EEquipmentType.Sword:
            case EEquipmentType.DoubleSword:
            case EEquipmentType.GreatSword:
            case EEquipmentType.Dagger:
            case EEquipmentType.Axe:
            case EEquipmentType.Stick:
            case EEquipmentType.MagicStick:
                ch.Wear(GetHand(side), equipment);
                break;
            default:
                Debug.Log("穿戴类型");
                break;
        }
    }

    private EPartOfBodyType GetHand(EBodySide side)
    {
        return side == EBodySide.Left ? EPartOfBodyType.LeftHand : EPartOfBodyType.RightHand;
    }
}

