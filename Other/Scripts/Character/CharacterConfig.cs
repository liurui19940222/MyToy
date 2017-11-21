using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "char_cfg_", menuName = "配置/角色")]
public class CharacterConfig : ScriptableObject {

    public int Id;

    public string PrefabName;

    public Vector3 FollowPoint;

    public AudioClip StepVoiceSound;

}
