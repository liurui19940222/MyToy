using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using DG.Tweening;

public class UIMainHUD : IUserInterface
{
    private Transform m_Lock;               //锁定图标
    private Transform m_BeLockTarget;       //被锁定目标
    private Vector3 m_LockOffset;           //锁定时相对目标原点的偏移量

    public UIMainHUD() : base("UIMainHUD", EUIType.HUD)
    {
    }

    protected override void OnInitialize()
    {
        base.OnInitialize();
        m_Lock = Transform.FindChild("imgLock");
    }

    public void LockTarget(Transform trans, Vector3 offset)
    {
        m_BeLockTarget = trans;
        m_LockOffset = offset;
        m_Lock.gameObject.SetActive(true);
    }

    public void UnlockTarget()
    {
        m_BeLockTarget = null;
        m_Lock.gameObject.SetActive(false);
    }

    public override void OnUpdate()
    {
        base.OnUpdate();
        if (m_BeLockTarget != null)
        {
            Vector3 screenPos = UIManager.Instance.Camera.WorldToScreenPoint(m_BeLockTarget.position + m_LockOffset);
            m_Lock.position = screenPos;
        }
    }
}

