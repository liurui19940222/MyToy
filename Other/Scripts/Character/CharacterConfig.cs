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
    public float Radius = 0.5f;             //半径
    public Vector3 CenterPos;               //中心店
    public float AttackDistance;            //攻击距离
    public AttackSequence[] AttackSequence; //攻击序列

    [HideInInspector]
    public int[] DefaultEquipments;         //默认装备
}

[System.Serializable]
public class AttackSequence
{
    public int[] Skills;                    //技能序列
    public ECondition Condition;            //触发条件
    public float Param;                     //参数

    public enum ECondition
    {
        Always,                             //总是可以
        Rate,                               //概率触发
        HP_Greater,                         //血量大于
        HP_Less,                            //血量小于
        Distance_Greater,                   //距离大于
        Distance_Less,                      //距离小于
    }
}
