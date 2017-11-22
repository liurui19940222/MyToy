using System;

public abstract class Hand : IPartOfBody
{
    public abstract int HandleInput(EInputWord word, bool down);
}

public class LeftHand : Hand
{
    public override int HandleInput(EInputWord word, bool down)
    {
        return 0;
    }
}

public class RightHand : Hand
{
    public override int HandleInput(EInputWord word, bool down)
    {
        return 0;
    }
}

