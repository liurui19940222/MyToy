using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RPGGame : Singleton<RPGGame>
{
    private CharacterSystem m_CharacterSystem;   //角色系统
    private SkillSystem m_SkillSystem;           //技能系统
    private IGameCamera m_GameCamera;            //游戏相机

    private UIMainHUD m_MainHUD;                 //主HUD界面

    public void Initialize()
    {
        //相机必须先被初始化
        m_GameCamera = new ThirdPersonCamera(this);
        m_CharacterSystem = new CharacterSystem(this);
        m_SkillSystem = new SkillSystem(this);

        //创建HUD
        m_MainHUD = UIManager.Instance.OpenUI<UIMainHUD>();

        InputSystem.Instance.AddInputEvent(OnInput, InputConst.INPUT_PRIORITY_WORLD);
    }

    public void OnUpdate()
    {
        m_CharacterSystem.OnUpdate();
        m_GameCamera.OnUpdate();
        m_SkillSystem.OnUpdate();
    }

    public void Quit()
    {
        //技能在角色之前释放
        m_SkillSystem.OnRelease();
        m_GameCamera.OnRelease();
        m_CharacterSystem.OnRelease();

        InputSystem.Instance.RemoveInputEvent(OnInput);
    }

    //得到游戏相机
    public IGameCamera GetCamera()
    {
        return m_GameCamera;
    }

    //执行技能
    public Skill ExecuteSkill(ICharacter ch, int skillId, IEquipment byWhichEquip, ICharacter target = null)
    {
        return m_SkillSystem.Execute(ch, skillId, byWhichEquip, target);
    }

    //创建玩家角色
    public void CreatePlayer()
    {
        m_CharacterSystem.CreatePlayer();
    }

    //得到玩家角色
    public ICharacter GetPlayer()
    {
        return m_CharacterSystem.GetPlayer();
    }

    //创建NPC
    public ICharacter CreateNPC<T>(int id, Vector3 position, Quaternion rotation) where T : ICharacter, new()
    {
        return m_CharacterSystem.CreateNPC<T>(id, position, rotation);
    }

    //设置AI
    public void SetAI(ICharacter ch, int fsmId)
    {
        m_CharacterSystem.SetAI(ch, fsmId);
    }

    //创建NPC并设置AI
    public void CreateNPCAndSetAI<T>(int id, Vector3 position, Quaternion rotation, int fsmId) where T : ICharacter, new()
    {
        ICharacter ch = CreateNPC<T>(id, position, rotation);
        m_CharacterSystem.SetAI(ch, fsmId);
    }

    //得到一个指定方向扇形范围内的角色
    public ICharacter GetACharacterWithFanShape(ICharacter self, Vector3 dir, float angle, float distance)
    {
        return m_CharacterSystem.GetACharacterWithFanShape(self, dir, angle, distance);
    }

    //得到指定方向扇形范围内的所有角色
    public List<ICharacter> GetCharactersWithFanShape(ICharacter self, Vector3 dir, float angle, float distance)
    {
        return m_CharacterSystem.GetCharactersWithFanShape(self, dir, angle, distance);
    }

    //显示锁定敌人HUD
    public void ShowLockTargetHUD(Transform target, Vector3 offset)
    {
        m_MainHUD.LockTarget(target, offset);
    }

    //取消显示锁定敌人HUD
    public void HideLockTargetHUD()
    {
        m_MainHUD.UnlockTarget();
    }

    //向子系统发送消息
    public void SendMessage(ESubsystem subsystem, WorldMessage msg)
    {
        switch (subsystem)
        {
            case ESubsystem.CharacterSystem:
                m_CharacterSystem.OnRecMessage(msg);
                break;
            case ESubsystem.SkillSystem:
                m_SkillSystem.OnRecMessage(msg);
                break;
            case ESubsystem.IGameCamera:
                m_GameCamera.OnRecMessage(msg);
                break;
        }
    }

    //监听控制器输入
    WorldMessage m_InputMsg = new WorldMessage();
    private bool OnInput(EInputWord word, bool down)
    {
        m_InputMsg.Reset();
        m_InputMsg.MessageId = MessageDefines.MSG_CHS_ONINPUT;
        m_InputMsg["word"] = word;
        m_InputMsg["down"] = down;
        SendMessage(ESubsystem.CharacterSystem, m_InputMsg);
        return true;
    }
}

public class IGameSystem
{
    protected RPGGame m_RPGGame;

    public IGameSystem(RPGGame game) {
        m_RPGGame = game;
        OnInitialize();
    }

    public virtual void OnInitialize() { }

    public virtual void OnUpdate() { }

    public virtual void OnRelease() { }

    public virtual void OnRecMessage(WorldMessage msg) { }
}

public enum ESubsystem
{
    CharacterSystem,
    SkillSystem,
    IGameCamera,
}
