using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class ICharacter
{
    private static int identity;

    private int m_InstanceId;                         //唯一Id
    protected GameObject m_GameObject;                //GameObject
    protected Transform m_Transform;                  //Transform
    protected Animator m_Animator;                    //Animator
    protected CharacterController m_Controller;       //角色控制器
    protected INavengation m_Navengation;             //寻路组件
    protected CharacterFSMMachine m_FSM;              //状态机
    protected CharacterConfig m_Config;               //角色配置
    protected IGameCamera m_GameCamera;               //跟随该角色的相机
    protected AnimationBehaviour m_AnimBehaviour;     //动画回调脚本
    private bool m_Stiff;                             //是否处于僵直状态
    private Vector3 m_Velocity;

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

    public CharacterFSMMachine FSM
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

    public bool Stiff
    {
        get { return m_Stiff; }
        set { m_Stiff = value; }
    }

    public virtual void OnInitialize() { }

    public virtual void OnUpdate()
    {
        //状态机更新
        if (m_FSM != null)
            m_FSM.OnUpdate();
        //寻路组件更新
        if (m_Navengation != null)
            m_Navengation.OnUpdate();

        UpdateGravity();
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

    public virtual void OnDestroy() { }

    public virtual void OnRecMessage(WorldMessage msg)
    {
        if (msg.MessageId == MessageDefines.MSG_CHS_ONINPUT)
            m_FSM.HandleInput(EFSMInputType.ControllerInput, msg);
    }

    //角色往指定方向走动
    float m_LastSpeed = 0;
    public void Move(Vector3 dir)
    {
        float speed = dir.magnitude;
        if (m_AnimBehaviour) m_AnimBehaviour.MoveSpeed = speed;
        if (speed < 0.15f)
        {
            m_LastSpeed = Mathf.Lerp(m_LastSpeed, 0, Time.deltaTime * 10);
            m_Animator.SetFloat("Speed", m_LastSpeed, 0.1f, Time.deltaTime);
            return;
        }

        dir = m_GameCamera.TransformDirection(dir);
        Quaternion rot = Quaternion.LookRotation(dir, Vector3.up);
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
        m_LastSpeed = speed;
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

    //设置到待机状态
    public virtual void SetToIdleState() { }
}
