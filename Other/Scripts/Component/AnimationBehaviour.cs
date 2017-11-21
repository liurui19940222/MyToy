using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AnimationBehaviour : MonoBehaviour
{

    private const string STEP_VOICE = "step";

    private AudioClip m_StepVoiceSound;         //脚步声音效
    private float m_LastPlayStepVoiceSoundTime; //最后一次播放脚步声的时间，用来避免动画混合的一些坑

    public AudioClip StepVoiceSound
    {
        get { return m_StepVoiceSound; }
        set { m_StepVoiceSound = value; }
    }

    public void AnimationEvent(string param)
    {
        if (param == STEP_VOICE)
        {
            if (StepVoiceSound != null)
            {
                if (Time.time - m_LastPlayStepVoiceSoundTime > 0.3f)
                {
                    SoundManager.Instance.PlaySound(StepVoiceSound);
                    m_LastPlayStepVoiceSoundTime = Time.time;
                }
            }
        }
    }
}
