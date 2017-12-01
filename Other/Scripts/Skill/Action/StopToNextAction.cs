using System.Collections.Generic;
using UnityEngine;

public class StopToNextAction : ISkillAction
{
    public StopToNextAction(Skill owner, Object m_Params) : base(owner, m_Params)
    {
    }

    public override void Execute()
    {
        Debug.Log("结束连击机会");
        m_Owner.ReadyToNext = false;
    }

    protected override void UnpackParams(Object _params)
    {

    }
}

