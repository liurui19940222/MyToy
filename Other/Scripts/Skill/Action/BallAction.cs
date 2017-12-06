using UnityEngine;

public class BallAction : ISkillAction
{
    private BallActionParams _Params;
    private Vector3 m_Velocity;
    private Vector3 m_AcceleratedSpeed;
    private Transform m_Transform;
    private Vector3 m_FowardDir;
    private float m_Timer;

    public BallAction(Skill owner, Object m_Params) : base(owner, m_Params)
    {
    }

    public override void Execute()
    {
        if (m_Owner.TargetCharacter != null)
            m_FowardDir = (m_Owner.TargetCharacter.Position - m_Owner.Character.Position).normalized;
        else
            m_FowardDir = m_Owner.Character.GetForward();
        m_Velocity = _Params.Force * m_FowardDir * Time.deltaTime;
        Transform bornPos = GetBindPos(_Params.BornPos);
        m_Transform = EffectManager.Instance.CreateEffect(_Params.BallEffectName, bornPos.position, Quaternion.LookRotation(m_FowardDir), -1.0f).transform;
    }

    public override bool OnUpdate()
    {
        m_AcceleratedSpeed = m_FowardDir * (_Params.Force / _Params.Mass);
        if (m_Owner.TargetCharacter != null)
            m_AcceleratedSpeed += ((m_Owner.TargetCharacter.Position - m_Owner.Character.Position).normalized - m_FowardDir) * _Params.AttractiveForce;
        m_Velocity += m_AcceleratedSpeed * Time.deltaTime;
        m_Transform.position += m_Velocity;
        m_Transform.forward = m_Velocity;

        if (CheckTime())
        {
            Debug.Log("时间到了");
            EffectManager.Instance.DestroyEffect(m_Transform.gameObject);
            return true;
        }
        if (DetectCollision())
        {
            return true;
        }
        return false;
    }

    protected override void UnpackParams(Object _params)
    {
        _Params = _params as BallActionParams;
    }

    //检查有没有超过生存时间
    private bool CheckTime()
    {
        m_Timer += Time.deltaTime;
        return m_Timer >= _Params.LifeTime;
    }

    //碰撞检测
    private bool DetectCollision()
    {
        bool hasCollision = false;
        foreach (RaycastHit hit in Physics.SphereCastAll(m_Transform.position, _Params.Radius, m_Transform.forward, 0.01f))
        {
            if (hit.transform.tag == GameConfig.Instance.Tags.Character)
            {
                ICharacter ch = hit.transform.GetComponent<PlayerMonoScript>().Character;
                if (ch == m_Owner.Character)
                    continue;
                Debug.Log("球体击中敌人:" + hit.transform.name);
                AttackEnemy(ch);
                hasCollision = true;
            }
            else if (hit.transform.tag == GameConfig.Instance.Tags.Obstacle)
            {
                hasCollision = true;
            }
        }
        if (hasCollision)
        {
            if (!string.IsNullOrEmpty(_Params.ExplosionEffectName))
                EffectManager.Instance.CreateEffect(_Params.ExplosionEffectName, m_Transform.position, Quaternion.identity, 3.0f);
            if (!string.IsNullOrEmpty(_Params.ExplosionSoundName))
                SoundManager.Instance.PlaySoundAtPoint(_Params.ExplosionSoundName, m_Transform.position);
            EffectManager.Instance.DestroyEffect(m_Transform.gameObject);
        }
        return hasCollision;
    }

    /// <summary>
    /// 攻击一个敌人
    /// </summary>
    /// <returns>是否攻击成功(没有被格挡或者闪避)</returns>
    private bool AttackEnemy(ICharacter enemy)
    {
        ICharacter.HurtByWhichSide dir = ICharacter.HurtByWhichSide.Forward;
        if (Vector3.Dot(enemy.GetForward(), (m_Transform.position - enemy.Position).normalized) > 0) //夹角小于90，说明被正前方的敌人所伤，受伤方向是向后
            dir = ICharacter.HurtByWhichSide.Back;
        if (enemy.TryHurt(m_Owner.ByWhichEquip, dir))
        {
            m_Owner.System.BreakAllSkills(enemy.InstanceId);
            return true;
        }
        return false;
    }
}

