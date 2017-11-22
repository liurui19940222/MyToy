public class ControlledState : CharacterState
{
    protected Player m_Player;

    public ControlledState(int id, BattleCharacter ch) : base(id, ch)
    {
        m_Player = ch as Player;
    }
}

