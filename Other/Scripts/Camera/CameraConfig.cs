using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "camera_cfg", menuName = "配置/相机")]
public class CameraConfig : ScriptableObject {

    public float Distance;     //与角色之间的距离
    public float RawAngle;     //初始角度(Pitch轴)
    public float RotateFactor; //旋转系数

}
