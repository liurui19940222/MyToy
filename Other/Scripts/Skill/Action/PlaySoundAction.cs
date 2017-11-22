using UnityEngine;

public class PlaySoundAction : ISkillAction
{
    PlaySoundActionParams m_Param;     //参数

    public PlaySoundAction(Skill owner, UnityEngine.Object m_Params) : base(owner, m_Params)
    {
    }

    public override void Execute()
    {
        AudioClip clip = ResourceFactory.Instance.LoadSound(m_Param.SoundName);
        if (clip)
        {
            SoundManager.Instance.PlaySound(clip);
        }
    }

    protected override void UnpackParams(UnityEngine.Object _params)
    {
        m_Param = _params as PlaySoundActionParams;
    }
}
