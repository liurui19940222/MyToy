using System.Collections.Generic;
using UnityEngine;

public class CheckHurtAction : ISkillAction
{
    private ICollitionDetection _Params;

    public CheckHurtAction(Skill owner, Object m_Params) : base(owner, m_Params)
    {
    }

    public override void Execute()
    {
        if (_Params.GetCollisionType() == ECollisionRange.Fan)
        {
            FanShapeCollisionParams p = _Params as FanShapeCollisionParams;
            List<ICharacter> chs =  RPGGame.Instance.GetCharactersWithFanShape(m_Owner.Character, m_Owner.Character.GetForward(), p.Angle, p.Distance);
            foreach (ICharacter ch in chs)
            {
                ICharacter.HurtByWhichSide dir = ICharacter.HurtByWhichSide.Forward;
                if (Vector3.Dot(ch.GetForward(), (m_Owner.Character.Position - ch.Position).normalized) > 0) //夹角小于90，说明被正前方的敌人所伤，受伤方向是向后
                    dir = ICharacter.HurtByWhichSide.Back;
                if (ch.TryHurt(m_Owner.ByWhichEquip, dir))
                    m_Owner.System.BreakAllSkills(ch.InstanceId);
            }
        }
    }

    protected override void UnpackParams(Object _params)
    {
        _Params = _params as ICollitionDetection;
    }
}

