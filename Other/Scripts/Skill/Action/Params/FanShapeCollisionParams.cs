using System;
using UnityEngine;

[CreateAssetMenu(fileName = "fan_shape_params", menuName = "技能参数/扇形碰撞检测")]
public class FanShapeCollisionParams : ScriptableObject, ICollitionDetection
{
    public Vector3 DirectionOffset;
    public float Distance;
    public float Angle;

    public bool IsCollision(ICollisionObject a, ICollisionObject b)
    {
        return MathEx.DetectFanCollision(b.GetPosition(), a.GetPosition(), a.GetForward() + DirectionOffset, Angle, Distance);
    }

    public ECollisionRange GetCollisionType()
    {
        return ECollisionRange.Fan;
    }
}

