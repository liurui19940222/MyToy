using System.Collections.Generic;
using UnityEngine;

public class CheckHurtAction : ISkillAction
{
    private ICollitionDetection _Params;
    private bool m_RayTest;
    private float m_Timer;
    private HashSet<int> m_Set;
    private List<Vector3> m_LastFramePoints;
    private bool m_HaveBeenBreak;

    public CheckHurtAction(Skill owner, Object m_Params) : base(owner, m_Params)
    {
    }

    public override void Execute()
    {
        if (_Params.GetCollisionType() == ECollisionRange.Fan)
        {
            FanShapeCollisionParams p = _Params as FanShapeCollisionParams;
            List<ICharacter> chs = RPGGame.Instance.GetCharactersWithFanShape(m_Owner.Character, m_Owner.Character.GetForward(), p.Angle, p.Distance);
            foreach (ICharacter ch in chs)
            {
                AttackEnemy(ch);
            }
        }
        else if (_Params.GetCollisionType() == ECollisionRange.ByBoneAnimation)
        {
            m_RayTest = true;
            m_Set = new HashSet<int>();
            m_LastFramePoints = new List<Vector3>();
            Util.ClearDebug();
        }
    }

    protected override void UnpackParams(Object _params)
    {
        _Params = _params as ICollitionDetection;
    }

    /// <summary>
    /// 攻击一个敌人
    /// </summary>
    /// <returns>是否攻击成功(没有被格挡或者闪避)</returns>
    private bool AttackEnemy(ICharacter enemy)
    {
        ICharacter.HurtByWhichSide dir = ICharacter.HurtByWhichSide.Forward;
        if (Vector3.Dot(enemy.GetForward(), (m_Owner.Character.Position - enemy.Position).normalized) > 0) //夹角小于90，说明被正前方的敌人所伤，受伤方向是向后
            dir = ICharacter.HurtByWhichSide.Back;
        if (enemy.TryHurt(m_Owner.ByWhichEquip, dir))
        {
            m_Owner.System.BreakAllSkills(enemy.InstanceId);
            return true;
        }
        return false;
    }

    public override bool OnUpdate()
    {
        if (m_HaveBeenBreak)
            return true;
        if (m_RayTest)
        {
            Weapon weapon = m_Owner.ByWhichEquip as Weapon;
            //第一帧的时候没有上一帧的点数据，不做碰撞检测
            if (m_LastFramePoints.Count == 0)
            {
                m_LastFramePoints = weapon.GetHitPoints();
            }
            else
            {
                Vector3 direction = default(Vector3);
                List<Vector3> points = weapon.GetHitPoints();
                for (int i = 0; i < m_LastFramePoints.Count; ++i)
                {
                    direction = points[i] - m_LastFramePoints[i];
                    Ray ray = new Ray(m_LastFramePoints[i], direction);
                    //Util.DebugCube(m_LastFramePoints[i], Vector3.one * 0.1f, Color.blue);
                    //Util.DebugCube(points[i], Vector3.one * 0.1f, Color.red);
                    foreach (RaycastHit hit in Physics.RaycastAll(ray, direction.magnitude))
                    {
                        if (m_Set.Contains(hit.transform.GetInstanceID()))
                            continue;
                        if (hit.transform.tag == GameConfig.Instance.Tags.Obstacle)
                        {
                            Debug.Log("击中障碍物:" + hit.transform.name);
                        }
                        else if (hit.transform.tag == GameConfig.Instance.Tags.Character)
                        {
                            Debug.Log("击中敌人:" + hit.transform.name);
                            if (AttackEnemy(hit.transform.GetComponent<PlayerMonoScript>().Character))
                                EffectManager.Instance.CreateBlood(EffectManager.Blood.Red_Normal, hit.point, Quaternion.LookRotation(hit.normal));
                        }
                        m_Set.Add(hit.transform.GetInstanceID());

                        //Util.DebugCube(hit.point, Vector3.one * 0.2f, Color.yellow);
                    }
                }
                m_LastFramePoints = points;
            }
            m_Timer += Time.deltaTime;
            if (m_Timer > (_Params as RayTestCollisionParams).Duration)
                return true;
            return false;
        }
        return base.OnUpdate();
    }

    public override void Break()
    {
        base.Break();
        m_HaveBeenBreak = true;
    }
}

