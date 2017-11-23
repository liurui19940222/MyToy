using UnityEngine;

public abstract class IGameCamera : IGameSystem
{
    protected Camera m_Camera;          //unity相机组件
    protected GameObject m_GameObject;   //GameObject
    protected Transform m_Transform;    //Transform
    protected ICharacter m_Character;   //跟随的角色
    protected ICharacter m_LockedEnemy; //被锁定的敌人

    public IGameCamera(RPGGame game) : base(game) { }

    public override void OnInitialize()
    {
        base.OnInitialize();
        m_Camera = Camera.main;
        m_GameObject = m_Camera.gameObject;
        m_Transform = m_Camera.transform;
    }

    //变换局部方向到世界空间
    public Vector3 TransformDirection(Vector3 dir)
    {
        float mag = dir.magnitude;
        dir = m_Transform.TransformDirection(dir);
        dir.y = 0;
        return dir.normalized * mag;
    }

    //跟随角色
    public void FollowCharacter(ICharacter character)
    {
        m_Character = character;
        m_Character.GameCamera = this;
    }

    //锁定敌人
    public void LockEnemy(ICharacter ch)
    {
        if (m_Character == null)
            return;
        m_LockedEnemy = ch;
    }

    //取消锁定敌人
    public void UnlockEnemy()
    {
        m_LockedEnemy = null;
    }

    //解除跟随
    public void CancelFollow()
    {
        m_Character = null;
    }

    //设置位置
    public void SetPosition(Vector3 position)
    {
        m_Transform.position = position;
    }

    //设置旋转
    public void SetRotation(Quaternion q)
    {
        m_Transform.rotation = q;
    }

    //注视
    public void LookAt(Vector3 target)
    {
        m_Transform.LookAt(target);
    }

    //处理控制器输入
    public abstract void HandleAxis(Vector2 axis);

    public abstract override void OnUpdate();
}

