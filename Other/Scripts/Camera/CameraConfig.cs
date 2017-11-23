using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "camera_cfg", menuName = "配置/相机")]
public class CameraConfig : ScriptableObject {

    public float Distance;              //与角色之间的距离
    public float RawAngle;              //初始角度(Pitch轴)
    public float RotateFactor;          //旋转系数
    public float LockEnemyHeight;       //锁定敌人时相机的高度
    public float LockEnemyDistance;     //锁定敌人时相机的距离
    public float LockEnemyMaxAngle;     //锁定敌人时相机的正方向与地面的最大夹角
}
