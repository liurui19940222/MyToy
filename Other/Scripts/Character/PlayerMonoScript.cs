using UnityEngine;

public class PlayerMonoScript : MonoBehaviour
{
    private ICharacter m_Character;

    public ICharacter Character
    {
        get { return m_Character; }
        set { m_Character = value; }
    }

    public Transform LeftHand;
    public Transform RightHand;
    public Transform Waist;
}

