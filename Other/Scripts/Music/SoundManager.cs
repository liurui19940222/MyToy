using UnityEngine;

public class SoundManager : SingletonMono<SoundManager>
{
    public void PlaySound(AudioClip clip)
    {
        GameObject go = new GameObject("sound-" + clip.name);
        AudioSource source = go.AddComponent<AudioSource>();
        source.clip = clip;
        source.Play();
        Destroy(go, clip.length);
    }
}

