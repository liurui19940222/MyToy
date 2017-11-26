using System;
using UnityEngine;

public class ThirdPersonCamera : IGameCamera
{
    private Vector2 m_Axis;         //输入的旋转量
    private Vector3 m_RawVector;    //初始向量
    private Vector3 m_RelativePos;  //相机与角色的相对位置
    private CameraConfig m_Config;  //相机配置

    private bool m_bInit;

    public ThirdPersonCamera(RPGGame game) : base(game)
    {
        m_Config = ResourceFactory.Instance.LoadConfig<CameraConfig>("camera_cfg");
        float rad = m_Config.RawAngle * Mathf.Deg2Rad;
        m_RawVector.y = Mathf.Sin(rad) * m_Config.Distance;
        m_RawVector.z = -Mathf.Cos(rad) * m_Config.Distance;
    }

    public override void OnRelease()
    {
        base.OnRelease();
        m_bInit = false;
    }

    public override void OnUpdate()
    {
        if (m_Character != null)
        {
            if (m_LockedEnemy == null)
            {
                Vector3 targetPos = m_Character.Position + m_Character.FollowPoint;
                if (!m_bInit)
                {
                    SetPosition(targetPos + m_RawVector);
                    m_RelativePos = targetPos - m_Transform.position;
                    m_bInit = true;
                }
                if (m_Axis == Vector2.zero)
                {
                    m_Transform.position = targetPos - m_RelativePos;
                    m_Transform.LookAt(targetPos);
                    return;
                }

                Quaternion rotation = Quaternion.AngleAxis(m_Axis.x, Vector3.up);
                Quaternion rise = Quaternion.AngleAxis(-m_Axis.y, Vector3.Cross((m_Transform.position - targetPos).normalized, Vector3.up));
                Vector3 point = m_Transform.position - targetPos;
                Vector3 euler = m_Transform.eulerAngles;
                euler.x = euler.x > 180 ? -(360 - euler.x) : euler.x;
                if (euler.x > 30 && m_Axis.y < 0)
                    rise = Quaternion.identity;
                if (euler.x < -20 && m_Axis.y > 0)
                    rise = Quaternion.identity;
                point = rotation * rise * point;
                m_Transform.position = targetPos + point.normalized * m_Config.Distance;
                m_Transform.LookAt(targetPos);
                m_RelativePos = targetPos - m_Transform.position;
            }
            else
            {
                Vector3 lockPos = m_LockedEnemy.Transform.position + m_LockedEnemy.CenterPosition;
                Vector3 targetPos = m_Character.Transform.position;
                targetPos.y = m_Config.LockEnemyHeight;
                Vector3 dir = targetPos - lockPos;
                float dis = m_Config.LockEnemyDistance;
                //限制一下距离特别近时的角度
                //float angle = Vector3.Angle(dir, m_Character.Transform.position - lockPos);
                //if (angle > m_Config.LockEnemyMaxAngle)
                //{
                //    dis += (angle - m_Config.LockEnemyMaxAngle) * 0.05f;
                //    dir = (m_Character.Transform.position - lockPos).normalized;
                //    dir = Quaternion.AngleAxis(m_Config.LockEnemyMaxAngle, Vector3.Cross(dir, Vector3.up)) * dir;
                //}
                //else
                    dir.Normalize();
                dir = dir * dis;
                m_Transform.position = Vector3.Lerp(m_Transform.position, targetPos + dir, Time.deltaTime * 5);
                m_Transform.LookAt(lockPos);
            }
        }
    }

    public override void HandleAxis(Vector2 axis)
    {
        m_Axis = axis * m_Config.RotateFactor * Time.deltaTime;
    }
}

