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
    }

    protected override void UnpackParams(Object _params)
    {
        _Params = _params as PlayAnimActionParams;
    }
}

[CreateAssetMenu(fileName = "play_anim_params", menuName = "技能参数/动作播放")]
public class PlayAnimActionParams : ScriptableObject
{
    public AnimType Type;
    public string ConditionName;
    public string Value; 

    public enum AnimType
    {
        Release,
        Move,
        Idle,
        Sheild,
        Dead,
    }
}
