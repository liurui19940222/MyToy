public class CharacterFSMMachine : FSMMachine
{
    protected BattleCharacter m_Character;
    private RPGGame m_RPGGame;

    public BattleCharacter Character
    {
        get { return m_Character; }
    }

    protected RPGGame RPGGame
    {
        get { return m_RPGGame; }
    }

    public CharacterFSMMachine(BattleCharacter ch, RPGGame game)
    {
        m_Character = ch;
        m_RPGGame = game;
    }
}

