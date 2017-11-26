using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ResourceFactory : SingletonMono<ResourceFactory> {

    private const string SOUND_PATH = "Sound/";                     //音效路径
    private const string CHARACTER_PATH = "Prefab/Character/";      //角色预设路径
    private const string EQUIPMENT_PATH = "Prefab/Equipment/";      //装备预设路径
    private const string EFFECT_PATH = "Prefab/Effect/";            //特效路径
    private const string UI_PATH = "UI/";                           //UI路径
    private const string CH_CONFIG_PATH = "Config/Character/";      //角色配置路径
    private const string SKILL_CONFIG_PATH = "Config/Skill/";       //技能配置路径
    private const string EQUIP_CONFIG_PATH = "Config/Equip/";       //装备配置路径
    private const string FSM_CONFIG_PATH = "Config/FSM/";           //状态机配置路径
    private const string CONFIG_PATH = "Config/";                   //配置路径

    private Dictionary<string, Object> m_Assets;

    protected override void Awake()
    {
        base.Awake();
        m_Assets = new Dictionary<string, Object>();
    }

    /// <summary>
    /// 加载资源
    /// </summary>
    /// <typeparam name="T">资源类</typeparam>
    /// <param name="path">路径</param>
    /// <param name="name">名字</param>
    /// <returns>资源实例</returns>
    public T LoadAsset<T>(string path, string name) where T : Object
    {
        string full_name = path + name;
        if (m_Assets.ContainsKey(full_name))
            return m_Assets[full_name] as T;
        T obj = Resources.Load<T>(full_name);
        m_Assets.Add(full_name, obj);
        return obj;
    }

    /// <summary>
    /// 加载音效
    /// </summary>
    public AudioClip LoadSound(string name)
    {
        return LoadAsset<AudioClip>(SOUND_PATH, name);
    }

    /// <summary>
    /// 加载角色预设
    /// </summary>
    public Object LoadCharacter(string name)
    {
        return LoadAsset<Object>(CHARACTER_PATH, name);
    }

    /// <summary>
    /// 加载装备预设
    /// </summary>
    public Object LoadEquipment(int id)
    {
        return LoadAsset<Object>(EQUIPMENT_PATH, name);
    }

    /// <summary>
    /// 加载UI
    /// </summary>
    public Object LoadUI(string name)
    {
        return LoadAsset<Object>(UI_PATH, name);
    }

    /// <summary>
    /// 加载配置
    /// </summary>
    public T LoadConfig<T>(string name) where T : Object
    {
        return LoadAsset<T>(CONFIG_PATH, name);
    }

    /// <summary>
    /// 加载角色配置
    /// </summary>
    public CharacterConfig LoadCharacterConfig(int id)
    {
        return LoadAsset<CharacterConfig>(CH_CONFIG_PATH, "char_cfg_" + id);
    }

    /// <summary>
    /// 加载技能配置
    /// </summary>
    public SkillConfig LoadSkillConfig(int id)
    {
        return LoadAsset<SkillConfig>(SKILL_CONFIG_PATH, "skill_" + id);
    }

    /// <summary>
    /// 加载装备配置
    /// </summary>
    public EquipmentConfig LoadEquipConfig(int id)
    {
        return LoadAsset<EquipmentConfig>(EQUIP_CONFIG_PATH, "equip_" + id);
    }

    /// <summary>
    /// 加载状态机配置
    /// </summary>
    public FSMController LoadFSM(int id)
    {
        return LoadAsset<FSMController>(FSM_CONFIG_PATH, "fsm_" + id);
    }
}
