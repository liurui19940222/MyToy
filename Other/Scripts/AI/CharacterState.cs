
public class CharacterState : FSMState
{
    protected ICharacter m_Character;

    public CharacterState(int id, ICharacter ch) : base(id)
    {
        m_Character = ch;
    }
}

