public abstract class IDressingRoom
{
    public abstract void Wear(ICharacter ch, IEquipment equip, EBodySide side);

    public enum EBodySide
    {
        Left,
        Right,
    }
}

