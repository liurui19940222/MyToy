using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[FSMCallClass]
public class Enemy : NonPlayerCharacter
{
    private Vector3 m_LastTraceTarget;      //上一次的追踪目标位置
    private float m_LastTraceTime;          //上一次追踪的时间

    private float m_BeginToCurStateTime;      //开始攻击的时间
    private int m_AttackCounter;            //攻击计数

    private float m_BeginToIdleTime;        //开始Idle的时间
    private float m_ThingkingTime = 1.2f;   //思考时间

    private float m_RotateCostTime;        //攻击的延迟时间（考虑到需要转向）
    private float m_AttackEnterTime;        //进入攻击状态时的时间（与延迟时间作计算）
    private AttackSequence m_AttackSequence;//当前攻击序列
    private Skill m_CurSkill;               //当前正在释放的技能

    private float m_LastComputeParamsTime;

    private FSMController FSMController
    {
        get {
            return m_FSM as FSMController;
        }
    }

    public override void OnInitialize()
    {
        base.OnInitialize();
    }

    public override void OnUpdate()
    {
        base.OnUpdate();
        if (Time.time - m_LastComputeParamsTime > 2.5f)
        {
            m_LastComputeParamsTime = Time.time;
            FSMController.SetFloat("Distance", (Player.Position - Position).magnitude);
        }
    }

    [FSMCallMethod]
    public void IdleEnter()
    {
        m_BeginToIdleTime = Time.time;
    }

    [FSMCallMethod]
    public void UpdateIdle()
    {
        if (Time.time - m_BeginToIdleTime > m_ThingkingTime)
        {
            FSMController.SetBool("HaveAShield", GetEqiupment(EPartOfBodyType.LeftHand) != null);
            FSMController.SetTrigger("ThinkingHaveDone");
        }
        else
        {
            Move(Vector3.zero);
        }
    }

    [FSMCallMethod]
    public void IdleExit()
    {

    }

    [FSMCallMethod]
    public void UpdateTrace()
    {
        if (!CanDoAction)
            return;
        if (Time.time - m_LastTraceTime > 1)
        {
            m_LastTraceTime = Time.time;
            m_LastTraceTarget = Player.Position;
        }
        Vector3 dir = m_LastTraceTarget - Position;
        float distance = dir.magnitude;
        float speed = Mathf.Min(3.0f,distance / 1.6f);
        speed = distance < 1.5f ? 0.0f : speed;
        Move(dir / distance * speed);
        LockEnemy(Player);
    }

    [FSMCallMethod]
    public void AttackEnter()
    {
        m_AttackSequence = null;
        m_BeginToCurStateTime = Time.time;
        m_AttackCounter = 0;
        //检查各种攻击组合是否满足
        for (int i = 0; i < m_Config.AttackSequence.Length; ++i)
        {
            AttackSequence seq = m_Config.AttackSequence[i];
            if (seq.Condition == AttackSequence.ECondition.Always)
            {
                m_AttackSequence = seq;
                break;
            }
            else if (seq.Condition == AttackSequence.ECondition.Rate)
            {
                if (Random.Range(0.0f, 1.0f) < seq.Param)
                {
                    m_AttackSequence = seq;
                    break;
                }
            }
        }
        CalcRotateTime();
    }

    [FSMCallMethod]
    public void UpdateAttack()
    {
        if (Time.time - m_BeginToCurStateTime < m_RotateCostTime)
            return;
        if (m_AttackSequence == null)
        {
            Debug.LogError("no skill have been selected");
            FSMController.SetTrigger("AttackingHaveDone");
            return;
        }
        Move(Vector3.zero);

        if (m_AttackCounter < m_AttackSequence.Skills.Length && !IsHiting)
        {
            if ((m_CurSkill != null && m_CurSkill.ReadyToNext) || !IsSkillStiff)    //如果有上一次技能释放，找衔接机会，否则只能等僵直结束
            {
                m_CurSkill = RPGGame.Instance.ExecuteSkill(this, m_AttackSequence.Skills[m_AttackCounter], GetEqiupment(EPartOfBodyType.RightHand));
                Debug.Log("第" + m_AttackCounter + "次攻击");
                m_AttackCounter++;
            }
        }
        if (Time.time - m_BeginToCurStateTime > 1.5f && CanDoAction)
        {
            FSMController.SetTrigger("AttackingHaveDone");
        }
    }

    [FSMCallMethod]
    public void ShieldMoveEnter()
    {
        Defend(true);
    }

    private Vector3 m_LastMoveDir;
    [FSMCallMethod]
    public void ShieldMoveUpdate()
    {
        if (!CanDoAction)
            return;
        //if (Time.time - m_LastTraceTime > 0.5f)
        //{
            m_LastTraceTime = Time.time;
            m_LastTraceTarget = Player.Position;
        //}
        Vector3 dir = m_LastTraceTarget - Position;
        float distance = dir.magnitude;
        float speed = Mathf.Min(1.0f, distance / 1.6f);
        speed = distance < 1.5f ? 0.0f : speed;
        dir = dir / distance * speed;
        dir = Vector3.RotateTowards(m_LastMoveDir, dir, Time.deltaTime, Time.deltaTime);
        Move(dir);
        LockEnemy(Player);
        m_LastMoveDir = dir;
    }

    [FSMCallMethod]
    public void ShieldMoveExit()
    {
        Defend(false);
    }

    [FSMCallMethod]
    public void DefenceEnter()
    {
        m_BeginToCurStateTime = Time.time;
        Defend(true);
        CalcRotateTime();
    }

    [FSMCallMethod]
    public void DefenceUpdate()
    {
        if (Time.time - m_BeginToCurStateTime < m_RotateCostTime + 2)
            return;
        Move(Vector3.zero);
        CalcRotateTime(100);
    }

    //计算转向
    private void CalcRotateTime(float rotateSpeed = 400)
    {
        Vector3 ToPlayerDir = Player.Position - Position;
        if (Vector3.Angle(GetForward(), ToPlayerDir) > 45)
        {
            m_RotateCostTime = RotateToTarget(Quaternion.LookRotation(ToPlayerDir, Vector3.up), rotateSpeed);
        }
    }
}
