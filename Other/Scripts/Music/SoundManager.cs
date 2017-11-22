using UnityEngine;

public class SoundManager : SingletonMono<SoundManager>
{
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
}

