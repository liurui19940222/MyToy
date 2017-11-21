using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ResourceFactory : SingletonMono<ResourceFactory> {

    private const string SOUND_PATH = "Sound/";                     //音效路径
    private const string CHARACTER_PATH = "Prefab/Character/";      //角色预设路径
    private const string EFFECT_PATH = "Prefab/Effect/";            //特效路径
    private const string UI_PATH = "UI/";                           //UI路径
    private const string CH_CONFIG_PATH = "Config/Character/";      //角色配置路径
    private const string SKILL_CONFIG_PATH = "Config/Skill/";       //技能配置路径
    private const string CONFIG_PATH = "Config/";                   //配置路径

    private Dictionary<string, Object> m_Assets;

    protected override void Awake()
    {
        base.Awake();
        m_Assets = new Dictionary<string, Object>();
    }

    public T LoadAsset<T>(string path, string name) where T : Object
    {
        string full_name = path + name;
        if (m_Assets.ContainsKey(full_name))
            return m_Assets[full_name] as T;
        T obj = Resources.Load<T>(full_name);
        m_Assets.Add(full_name, obj);
        return obj;
    }

    public AudioClip LoadSound(string name)
    {
        return LoadAsset<AudioClip>(SOUND_PATH, name);
    }

    public Object LoadCharacter(string name)
    {
        return LoadAsset<Object>(CHARACTER_PATH, name);
    }

    public Object LoadUI(string name)
    {
        return LoadAsset<Object>(UI_PATH, name);
    }

    public T LoadConfig<T>(string name) where T : Object
    {
        return LoadAsset<T>(CONFIG_PATH, name);
    }

    public CharacterConfig LoadCharacterConfig(string name)
    {
        return LoadAsset<CharacterConfig>(CH_CONFIG_PATH, name);
    }

    public SkillConfig LoadSkillConfig(int id)
    {
        return LoadAsset<SkillConfig>(SKILL_CONFIG_PATH, "skill_" + id);
    }
}
