using UnityEngine;

public class PlayEffectAction : ISkillAction
{
    PlayEffectActionParams m_Param;     //参数

    public PlayEffectAction(Skill owner, UnityEngine.Object m_Params) : base(owner, m_Params)
    {
    }

    public override void Execute()
    {
        Object obj = ResourceFactory.Instance.LoadAsset<Object>(m_Param.EffectPath, m_Param.EffectName);
        if (obj == null)
            return;
        GameObject go = GameObject.Instantiate(obj, m_Param.PlayPosition, m_Param.PlayRotation) as GameObject;
        GameObject.Destroy(go, m_Param.DestroyTime);
    }

    protected override void UnpackParams(UnityEngine.Object _params)
    {
        m_Param = _params as PlayEffectActionParams;
    }
}

[CreateAssetMenu(fileName = "play_effect_params", menuName = "技能参数/特效播放")]
public class PlayEffectActionParams : ScriptableObject
{
    public string EffectPath = "Prefab/Effect/";    //特效路径
    public string EffectName;                       //特效名字
    public Vector3 PlayPosition;                    //播放位置
    public Quaternion PlayRotation;                 //播放旋转
    public float DestroyTime;                       //销毁时间
}
