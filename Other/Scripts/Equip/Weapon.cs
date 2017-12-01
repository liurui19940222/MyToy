using System.Collections.Generic;
using UnityEngine;

public class Weapon : IEquipment
{
    private List<Transform> m_RayPoints;                //碰撞点
    private Transform m_MagicCore;                      //魔法核

    public Weapon(EquipmentConfig config) : base(config)
    {
        m_RayPoints = new List<Transform>();
        InitRayPoints();
        m_MagicCore = Transform.FindChild("model/magic_core");
    }

    public List<Transform> RayPoints
    {
        get { return m_RayPoints; }
    }

    public Transform MagicCore
    {
        get { return m_MagicCore; }
    }

    private void InitRayPoints()
    {
        Transform rayPointsParent = Transform.FindChild("model/ray_point");
        if (!rayPointsParent)
            return;
        for (int i = 0; i < rayPointsParent.childCount; ++i)
        {
            RayPoints.Add(rayPointsParent.GetChild(i));
        }
    }

    public List<Vector3> GetHitPoints()
    {
        List<Vector3> list = new List<Vector3>();
        for (int i = 0; i < m_RayPoints.Count; ++i)
        {
            list.Add(m_RayPoints[i].position);
        }
        return list;
    }
}

