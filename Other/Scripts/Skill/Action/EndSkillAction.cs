using System.Collections.Generic;
using UnityEngine;

public class EndSkillAction : ISkillAction
{
    public EndSkillAction(Skill owner, Object m_Params) : base(owner, m_Params)
    {
    }

    public override void Execute()
    {
        m_Owner.Done = true;
        if (m_Owner.System.HasDone(m_Owner.Character.InstanceId))
        {
            m_Owner.Character.Stiff = false;
            m_Owner.Character.SetToIdleState();
        }
    }

    protected override void UnpackParams(Object _params)
    {
        
    }
}

