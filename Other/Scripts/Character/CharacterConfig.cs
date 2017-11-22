using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "char_cfg_", menuName = "配置/角色")]
public class CharacterConfig : ScriptableObject {

    public int Id;                          //Id

    public string PrefabName;               //预设名

    public Vector3 FollowPoint;             //跟随点

    public AudioClip StepVoiceSound;        //脚步声

    public float Gravity = 10;              //重力

    [HideInInspector]
    public int[] DefaultEquipments;         //默认装备
}
