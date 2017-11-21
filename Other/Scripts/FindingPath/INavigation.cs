using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class INavengation {

    private ICharacter m_Character;

    public abstract void Go(Vector3 targetPos);

    public abstract void Stop();

    public virtual void OnUpdate() { }
}
