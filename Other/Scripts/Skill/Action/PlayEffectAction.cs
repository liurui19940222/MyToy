using UnityEngine;

public class PlayEffectAction : ISkillAction
{
    PlayEffectActionParams m_Param;     //参数

    public PlayEffectAction(Skill owner, UnityEngine.Object m_Params) : base(owner, m_Params)
    {
    }

    public override void Execute()
    {
        Object obj = ResourceFactory.Instance.LoadAsset<Object>(m_Param.EffectPath, m_Param.EffectName);
        if (obj == null)
            return;
        GameObject go = GameObject.Instantiate(obj, m_Param.PlayPosition, m_Param.PlayRotation) as GameObject;
        GameObject.Destroy(go, m_Param.DestroyTime);
    }

    protected override void UnpackParams(UnityEngine.Object _params)
    {
        m_Param = _params as PlayEffectActionParams;
    }
}

