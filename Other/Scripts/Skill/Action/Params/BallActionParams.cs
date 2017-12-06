using System;
using UnityEngine;

[CreateAssetMenu(fileName = "ball_params", menuName = "技能参数/球体飞行碰撞物")]
public class BallActionParams : ScriptableObject
{
    public string BallEffectName;       //飞行物特效名字
    public string ExplosionEffectName;  //爆炸特效名字
    public string ExplosionSoundName;   //爆炸音效名字
    public EBindPos BornPos;            //出身位置
    public float Radius;                //半径
    public float LifeTime;              //生存时间
    public float Force;                 //施加的作用力
    public float Mass;                  //质量
    public float AttractiveForce;       //作用目标的吸引力（值越大越能跟踪目标，为0时直线向前）
}

