public abstract class IDressingRoom
{
    public abstract void Wear(BattleCharacter ch, IEquipment equip, EBodySide side);

    public enum EBodySide
    {
        Left,
        Right,
    }
}

