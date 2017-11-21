public class CharacterFSMMachine : FSMMachine
{
    protected ICharacter m_Character;
    private RPGGame m_RPGGame;

    public ICharacter Character
    {
        get { return m_Character; }
    }

    protected RPGGame RPGGame
    {
        get { return m_RPGGame; }
    }

    public CharacterFSMMachine(ICharacter ch, RPGGame game)
    {
        m_Character = ch;
        m_RPGGame = game;
    }
}

