using UnityEngine;

public class SoundManager : SingletonMono<SoundManager>
{
    private const int SOUND_HIT_SHARP_01 = 0;
    private const int SOUND_HIT_SHARP_02 = 1;
    private const int SOUND_HIT_SHARP_03 = 2;
    private const int SOUND_HIT_BLUNT_01 = 3;
    private const int SOUND_HIT_BLUNT_02 = 4;
    private const int SOUND_HIT_BLUNT_03 = 5;
    private const int SOUND_SHIELD_BLOCK_01 = 6;    //盾牌格挡（木制）
    private const int SOUND_SHIELD_BLOCK_02 = 7;    //盾牌格挡（钢制）

    private AudioClip[] m_Sounds;

    protected override void Awake()
    {
        base.Awake();
        m_Sounds = new AudioClip[8];
        m_Sounds[SOUND_HIT_SHARP_01] = ResourceFactory.Instance.LoadSound("sword_hit_01");
        m_Sounds[SOUND_HIT_BLUNT_01] = ResourceFactory.Instance.LoadSound("blunt_hit_01");
        m_Sounds[SOUND_SHIELD_BLOCK_01] = ResourceFactory.Instance.LoadSound("shield_block_hit_01");
        m_Sounds[SOUND_SHIELD_BLOCK_02] = ResourceFactory.Instance.LoadSound("shield_block_hit_02");
    }

    public void PlaySound(AudioClip clip, float volume)
    {
        GameObject go = new GameObject("sound-" + clip.name);
        AudioSource source = go.AddComponent<AudioSource>();
        source.clip = clip;
        source.volume = volume;
        source.Play();
        Destroy(go, clip.length);
    }

    public void PlaySound(AudioClip clip)
    {
        PlaySound(clip, 1.0f);
    }

    public void PlaySoundAtPoint(AudioClip clip, Vector3 point)
    {
        AudioSource.PlayClipAtPoint(clip, point);
    }

    public void PlaySoundAtPoint(string soundName, Vector3 point)
    {
        AudioClip clip = ResourceFactory.Instance.LoadSound(soundName);
        PlaySoundAtPoint(clip, point);
    }

    //播放受伤音效
    public void PlayHitSound(IEquipment byWhatWeapon, Vector3 position)
    {
        if (byWhatWeapon.IsBluntWeapon())
        {
            PlaySoundAtPoint(m_Sounds[SOUND_HIT_BLUNT_01], position);
        }
        else
        {
            PlaySoundAtPoint(m_Sounds[SOUND_HIT_SHARP_01], position);
        }
    }

    //播放格挡音效
    public void PlayShieldBlockSound(IEquipment shield, Vector3 position)
    {
        if (shield.Material == EEquipmentMateril.Wood)
        {
            PlaySoundAtPoint(m_Sounds[SOUND_SHIELD_BLOCK_01], position);
        }
        else if (shield.Material == EEquipmentMateril.Iron)
        {
            PlaySoundAtPoint(m_Sounds[SOUND_SHIELD_BLOCK_02], position);
        }
    }
}

