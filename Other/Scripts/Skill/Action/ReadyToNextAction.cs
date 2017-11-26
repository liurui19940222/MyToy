using System.Collections.Generic;
using UnityEngine;

public class ReadyToNextAction : ISkillAction
{
    public ReadyToNextAction(Skill owner, Object m_Params) : base(owner, m_Params)
    {
    }

    public override void Execute()
    {
        Debug.Log("准备好下一次");
        m_Owner.Character.StopMove();
        m_Owner.ReadyToNext = true;
    }

    protected override void UnpackParams(Object _params)
    {

    }
}

