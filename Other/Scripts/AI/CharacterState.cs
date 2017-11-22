
public class CharacterState : FSMState
{
    protected BattleCharacter m_Character;

    public CharacterState(int id, BattleCharacter ch) : base(id)
    {
        m_Character = ch;
    }
}

