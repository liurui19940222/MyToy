using UnityEngine;

[CreateAssetMenu(fileName = "play_effect_params", menuName = "技能参数/特效播放")]
public class PlayEffectActionParams : ScriptableObject
{
    public string EffectName;                       //特效名字
    public Vector3 PlayPosition;                    //播放位置
    public Quaternion PlayRotation;                 //播放旋转
    public float DestroyTime;                       //销毁时间
    public EBindPos BindPos;                        //绑定位置

    public enum EBindPos
    {
        None,
        MagicCore,
    }
}
