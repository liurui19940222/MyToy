using UnityEngine;

public class PlayAnimAction : ISkillAction
{
    private PlayAnimActionParams _Params;

    public PlayAnimAction(Skill owner, Object m_Params) : base(owner, m_Params)
    {
    }

    public override void Execute()
    {
        m_Owner.Character.PlayAnim(_Params.ConditionName);
        Debug.Log("anim " + _Params.ConditionName);
    }

    protected override void UnpackParams(Object _params)
    {
        _Params = _params as PlayAnimActionParams;
    }
}

