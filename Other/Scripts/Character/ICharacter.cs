using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class ICharacter : ICollisionObject
{
    private static int identity;

    private static IDressingRoom DressingRoom = new DressingRoom();

    private int m_InstanceId;                         //唯一Id
    protected GameObject m_GameObject;                //GameObject
    protected Transform m_Transform;                  //Transform
    protected Animator m_Animator;                    //Animator
    protected CharacterController m_Controller;       //角色控制器
    protected INavengation m_Navengation;             //寻路组件
    protected IFSMMachine m_FSM;                      //状态机
    protected CharacterConfig m_Config;               //角色配置
    protected IGameCamera m_GameCamera;               //跟随该角色的相机
    protected AnimationBehaviour m_AnimBehaviour;     //动画回调脚本
    protected PlayerMonoScript m_MonoScript;          //Mono脚本
    private bool m_IsSkillStiff;                      //是否处于僵直状态
    private bool m_IsLocking;                         //是否在锁定敌人
    private bool m_IsCombating;                       //是否正在战斗状态
    private bool m_IsDefending;                       //是否在防御状态
    private bool m_IsHiting;                          //是否正在被打击
    private float m_BeHitTime;                        //被攻击产生硬直的开始时间
    private ICharacter m_LockedEnemy;                 //被锁定的敌人
    private Vector3 m_Velocity;
    private IPartOfBody[] m_Body;                     //身体各部分

    private Quaternion m_TargetRotation;              //要转向的目标方向
    private Quaternion m_BeginRotation;               //转向前的方向
    private float m_RotateTimer;                      //转向计时
    private float m_RotateTime;                       //完成转向的总时间
    private bool m_IsRotating;                        //是否正在转向

    public ICharacter() { m_InstanceId = ++identity; }

    public int InstanceId
    {
        get { return m_InstanceId; }
    }

    public GameObject GameObject
    {
        get { return m_GameObject; }
        set { m_GameObject = value; }
    }

    public Transform Transform
    {
        get { return m_Transform; }
        set { m_Transform = value; }
    }

    public Animator Animator
    {
        get { return m_Animator; }
        set { m_Animator = value; }
    }

    public CharacterController Controller
    {
        get { return m_Controller; }
        set { m_Controller = value; }
    }

    public INavengation Navengation
    {
        get { return m_Navengation; }
        set { m_Navengation = value; }
    }

    public IFSMMachine FSM
    {
        get { return m_FSM; }
        set { m_FSM = value; }
    }

    public AnimationBehaviour AnimBehaviour
    {
        get { return m_AnimBehaviour; }
        set { m_AnimBehaviour = value; }
    }

    public Vector3 Position
    {
        get { return m_Transform.position; }
        set { m_Transform.position = value; }
    }

    public Quaternion Rotation
    {
        set { m_Transform.rotation = value; }
        get { return m_Transform.rotation; }
    }

    public Vector3 Scale
    {
        set { m_Transform.localScale = value; }
        get { return m_Transform.localScale; }
    }

    public Vector3 EulerAngles
    {
        set { m_Transform.eulerAngles = value; }
        get { return m_Transform.eulerAngles; }
    }

    public Vector3 FollowPoint
    {
        get { return Config.FollowPoint; }
    }

    public Vector3 CenterPosition
    {
        get { return Config.CenterPos; }
    }

    public CharacterConfig Config
    {
        get { return m_Config; }
        set { m_Config = value; }
    }

    public IGameCamera GameCamera
    {
        get { return m_GameCamera; }
        set { m_GameCamera = value; }
    }

    public bool IsSkillStiff
    {
        get { return m_IsSkillStiff; }
        set
        {
            m_IsSkillStiff = value;
        }
    }

    public bool IsLocking
    {
        get { return m_IsLocking; }
        set { m_IsLocking = value; }
    }

    public bool IsCombating
    {
        get { return m_IsCombating; }
        set { m_IsCombating = value; }
    }

    //能否行动
    public bool CanDoAction
    {
        get { return !m_IsSkillStiff && !IsHiting; }
    }

    public bool IsDefending
    {
        get { return m_IsDefending; }
        set { m_IsDefending = value; }
    }

    public ICharacter LockedEnemy
    {
        get { return m_LockedEnemy; }
        set { m_LockedEnemy = value; }
    }

    public bool IsHiting
    {
        get { return m_IsHiting; }
    }

    public PlayerMonoScript MonoScript
    {
        get { return m_MonoScript; }
        set { m_MonoScript = value; }
    }

    public virtual void OnInitialize()
    {
        m_Body = Factory.Instance.CreateBody();
        m_Body[(int)EPartOfBodyType.LeftHand].Transform = MonoScript.LeftHand;
        m_Body[(int)EPartOfBodyType.RightHand].Transform = MonoScript.RightHand;
        m_Body[(int)EPartOfBodyType.Waist].Transform = MonoScript.Waist;
    }

    public virtual void OnUpdate()
    {
        //状态机更新
        if (m_FSM != null)
            m_FSM.OnUpdate();
        //寻路组件更新
        if (m_Navengation != null)
            m_Navengation.OnUpdate();

        UpdateGravity();
        UpdateHit();
        UpdateRotation();
    }

    //更新重力
    private void UpdateGravity()
    {
        if (!m_Controller.isGrounded)
        {
            m_Velocity.y -= m_Config.Gravity * Time.deltaTime;
        }
        else
        {
            m_Velocity.y = 0;
        }
        m_Controller.Move(m_Velocity);
    }

    //更新受击状态
    private void UpdateHit()
    {
        if (!IsHiting)
            return;
        if (Time.time - m_BeHitTime > 1.2f)
        {
            m_IsHiting = false;
        }
    }

    public virtual void OnDestroy() { }

    public virtual void OnRecMessage(WorldMessage msg)
    {
        if (msg.MessageId == MessageDefines.MSG_CHS_ONINPUT)
            m_FSM.HandleInput(EFSMInputType.ControllerInput, msg);
    }


    //穿戴装备
    public void Wear(IEquipment equipment)
    {
        DressingRoom.Wear(this, equipment, IDressingRoom.EBodySide.Left);
    }

    //装备武器
    public void WearWeapon(IEquipment equipment, IDressingRoom.EBodySide side)
    {
        DressingRoom.Wear(this, equipment, side);
    }

    //穿戴装备
    public void Wear(EPartOfBodyType body, IEquipment equipment)
    {
        m_Body[(int)body].Euipment = equipment;
    }

    //得到一个部位的装备
    public IEquipment GetEqiupment(EPartOfBodyType part)
    {
        return m_Body[(int)part].Euipment;
    }

    //根据输入得到处理结果
    public HandleInputResult GetInputResult(EInputWord word, bool down)
    {
        HandleInputResult result = null;
        if (word == EInputWord.RB || word == EInputWord.RT)
        {
            result = m_Body[(int)EPartOfBodyType.RightHand].HandleInput(word, down);
            if (result != null) result.ByWitchHand = EPartOfBodyType.RightHand;
        }
        else if (word == EInputWord.LB || word == EInputWord.LT)
        {
            result = m_Body[(int)EPartOfBodyType.LeftHand].HandleInput(word, down);
            if (result != null) result.ByWitchHand = EPartOfBodyType.LeftHand;
        }
        return result;
    }

    //角色往指定方向走动
    float m_LastSpeed = 0;
    public void Move(Vector3 dir)
    {
        Vector3 rawDir = dir;
        float speed = dir.magnitude;
        SetAnimStateToLock(IsLocking);
        if (m_AnimBehaviour) m_AnimBehaviour.MoveSpeed = speed;
        if (speed < 0.15f)
        {
            m_LastSpeed = Mathf.Lerp(m_LastSpeed, 0, Time.deltaTime * 10);
            m_Animator.SetFloat("Speed", m_LastSpeed, 0.1f, Time.deltaTime);
            if (IsLocking)
            {
                m_Animator.SetFloat("xSpeed", m_LastSpeed / 1.414f, 0.5f, Time.deltaTime);
                m_Animator.SetFloat("zSpeed", m_LastSpeed / 1.414f, 0.5f, Time.deltaTime);
            }
            return;
        }
        if (this is Player)
            dir = m_GameCamera.TransformDirection(dir);
        else
            rawDir = Transform.InverseTransformDirection(dir);
        Quaternion rot = default(Quaternion);
        if (!IsLocking)
            rot = Quaternion.LookRotation(dir, Vector3.up);
        else
            rot = Quaternion.LookRotation(LockedEnemy.Position - Position, Vector3.up);
        float angle = Quaternion.Angle(rot, m_Transform.rotation);
        if (speed != 0)
        {
            m_Transform.rotation = Quaternion.Slerp(m_Transform.rotation, rot, Time.deltaTime * 20);
        }
        if (angle > 90)
        {
            speed = Mathf.Max(speed, 1.0f);
        }
        else
        {
            speed = Mathf.Lerp(m_LastSpeed, speed, Time.deltaTime * 15);
        }

        m_Controller.Move(dir.normalized * speed * Time.deltaTime);
        m_Animator.SetFloat("Speed", Mathf.Max(speed, 1.0f), 0.1f, Time.deltaTime);
        if (IsLocking)
        {
            m_Animator.SetFloat("xSpeed", rawDir.x, 0.1f, Time.deltaTime);
            m_Animator.SetFloat("zSpeed", rawDir.z, 0.1f, Time.deltaTime);
        }
        m_LastSpeed = speed;
    }

    //转到指定方向
    public float RotateToTarget(Quaternion targetRotation, float speed)
    {
        m_RotateTime = Quaternion.Angle(Rotation, targetRotation) / speed;
        if (m_RotateTime < float.Epsilon)
            return 0;
        m_IsRotating = true;
        m_RotateTimer = 0;
        m_TargetRotation = targetRotation;
        m_BeginRotation = Rotation;
        return m_RotateTime;
    }

    //更新转向
    private void UpdateRotation()
    {
        if (!m_IsRotating || !CanDoAction)
            return;
        m_RotateTimer += Time.deltaTime;
        if (m_RotateTimer > m_RotateTime)
        {
            m_IsRotating = false;
            return;
        }
        Rotation = Quaternion.Slerp(m_BeginRotation, m_TargetRotation, m_RotateTimer / m_RotateTime);
        m_Animator.SetFloat("Speed", 2, 0.1f, Time.deltaTime);
    }

    //防御
    public void Defend(bool doesDefend)
    {
        PlaySheildAnim(doesDefend);
        IsDefending = doesDefend;
    }

    //停止移动
    public void StopMove()
    {
        m_LastSpeed = 0;
        m_Animator.SetFloat("Speed", 0);
        m_Animator.SetFloat("xSpeed", 0);
        m_Animator.SetFloat("zSpeed", 0);
    }

    //播放移动动画，小于0.01时自动回到待机动画
    public void PlayMoveAnim(Vector3 dir)
    {
        m_Animator.SetFloat("Speed", dir.magnitude);
    }

    //播放防御动画
    public void PlaySheildAnim(bool isDefence)
    {
        m_Animator.SetBool("Defence", isDefence);
    }

    //播放战斗模式动画
    public void PlayCombatAnim(bool isCombat)
    {
        m_Animator.SetBool("Combat", isCombat);
    }

    //设置动画状态到锁定
    public void SetAnimStateToLock(bool isLock)
    {
        m_Animator.SetBool("Lock", isLock);
    }

    //播放动画
    public void PlayAnim(string trigger)
    {
        m_Animator.SetTrigger(trigger);
    }

    //检查技能条件
    public bool CheckSkillCondition(SkillCondition condition)
    {
        return true;
    }

    //受伤
    public bool TryHurt(IEquipment equipment, HurtByWhichSide dir)
    {
        if (IsDefending && dir == HurtByWhichSide.Back)
        {
            SoundManager.Instance.PlayShieldBlockSound(GetEqiupment(EPartOfBodyType.LeftHand), Position);
            m_Animator.SetTrigger("HurtOnSheild");
            m_Animator.SetFloat("HitStrength", 0.2f);
            EffectManager.Instance.CreateSpark(EffectManager.Spark.Iron, MonoScript.LeftHand.position, Quaternion.identity);
            return false;
        }
        else
        {
            if (dir == HurtByWhichSide.Back)
                m_Animator.SetTrigger("HurtByBack");
            else if (dir == HurtByWhichSide.Forward)
                m_Animator.SetTrigger("HurtByForward");
            if (equipment != null)
                SoundManager.Instance.PlayHitSound(equipment, Position);
            m_BeHitTime = Time.time;
            m_IsHiting = true;
            return true;
        }
    }

    //设置到待机状态
    public virtual void SetToIdleState() { }

    public Vector3 GetPosition()
    {
        return Position;
    }

    public Vector3 GetForward()
    {
        return Transform.forward;
    }

    public float GetRadius()
    {
        return Config.Radius;
    }

    public enum HurtByWhichSide
    {
        Back,
        Forward,
    }
}
