using UnityEngine;

public enum ECollisionRange
{
    ByBoneAnimation,        //根据骨骼动画
    Fan,                    //扇形
    Circle,                 //圆形
    Sphere,                 //球型
    Rectangle,              //矩形
    Linear,                 //线性
    Single,                 //单体
}

public interface ICollisionObject
{
    Vector3 GetPosition();

    Vector3 GetForward();

    float GetRadius();
}

public interface ICollitionDetection
{
    bool IsCollision(ICollisionObject a, ICollisionObject b);

    ECollisionRange GetCollisionType();
}
