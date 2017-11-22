using UnityEngine;

[CreateAssetMenu(fileName = "play_anim_params", menuName = "技能参数/动作播放")]
public class PlayAnimActionParams : ScriptableObject
{
    public AnimType Type;
    public string ConditionName;
    public string Value;

    public enum AnimType
    {
        Release,
        Move,
        Idle,
        Sheild,
        Dead,
    }
}
