using System;
using UnityEngine;

[CreateAssetMenu(fileName = "ray_test_params", menuName = "技能参数/射线碰撞检测")]
public class RayTestCollisionParams : ScriptableObject, ICollitionDetection
{
    public float Duration;      //持续时间

    public bool IsCollision(ICollisionObject a, ICollisionObject b)
    {
        return false;
    }

    public ECollisionRange GetCollisionType()
    {
        return ECollisionRange.ByBoneAnimation;
    }
}

