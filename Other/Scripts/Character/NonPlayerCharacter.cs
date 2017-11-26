public class NonPlayerCharacter : ICharacter
{
    private Player m_Player;
    protected Player Player
    {
        get
        {
            if (m_Player == null)
                m_Player = RPGGame.Instance.GetPlayer() as Player;
            return m_Player;
        }
    }

    public void LockEnemy(ICharacter ch)
    {
        IsLocking = true;
        LockedEnemy = ch;
    }

    public void UnlockEnemy()
    {
        IsLocking = false;
        LockedEnemy = null;
    }
}

