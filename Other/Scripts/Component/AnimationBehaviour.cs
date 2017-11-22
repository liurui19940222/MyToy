using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AnimationBehaviour : MonoBehaviour
{
    private const string STEP_VOICE = "step";
    private const string SWORD_WAVE_VOICE = "sword_wave_voice";

    private float m_MoveSpeed;                                      //移动速度，用来避免不适当的脚步声
    private AudioClip m_StepVoiceSound;                             //脚步声音效
    private float m_LastPlayStepVoiceSoundTime;                     //最后一次播放脚步声的时间，用来避免动画混合的一些坑

    public AudioClip StepVoiceSound
    {
        get { return m_StepVoiceSound; }
        set { m_StepVoiceSound = value; }
    }

    public float MoveSpeed
    {
        get { return m_MoveSpeed; }
        set { m_MoveSpeed = value; }
    }

    public void AnimationEvent(string param)
    {
        if (param == STEP_VOICE)
        {
            if (StepVoiceSound != null && m_MoveSpeed > 0.2f)
            {
                if (Time.time - m_LastPlayStepVoiceSoundTime > 0.3f)
                {
                    SoundManager.Instance.PlaySound(StepVoiceSound, Mathf.Clamp01(m_MoveSpeed));
                    m_LastPlayStepVoiceSoundTime = Time.time;
                }
            }
        }
    }
}
