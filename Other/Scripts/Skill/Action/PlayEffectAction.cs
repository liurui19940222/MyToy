using UnityEngine;

public class PlayEffectAction : ISkillAction
{
    PlayEffectActionParams m_Param;     //参数
    private GameObject m_Effect;

    public PlayEffectAction(Skill owner, UnityEngine.Object m_Params) : base(owner, m_Params)
    {
    }

    public override void Execute()
    {
        if (m_Param.BindPos == PlayEffectActionParams.EBindPos.None)
        {
            m_Effect = EffectManager.Instance.CreateEffect(m_Param.EffectName, m_Param.PlayPosition, m_Param.PlayRotation, m_Param.DestroyTime);
            return;
        }
        Transform parent = null;
        Weapon weapon = m_Owner.ByWhichEquip as Weapon;
        if (m_Param.BindPos == PlayEffectActionParams.EBindPos.MagicCore)
        {
            parent = weapon.MagicCore;
        }
        m_Effect = EffectManager.Instance.CreateEffect(parent, m_Param.EffectName, m_Param.PlayPosition, m_Param.PlayRotation, m_Param.DestroyTime);
    }

    protected override void UnpackParams(UnityEngine.Object _params)
    {
        m_Param = _params as PlayEffectActionParams;
    }

    public override void Break()
    {
        base.Break();
        if (m_Effect != null)
            EffectManager.Instance.DestroyEffect(m_Effect);
    }
}

