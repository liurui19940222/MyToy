using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CharacterSystem : IGameSystem {

    private ICharacter m_ControlledCharacter;   //当前受控制的角色

    private Dictionary<int, ICharacter> m_Characters;      //所有角色的集合

    public CharacterSystem(RPGGame game) : base(game)
    {
    }

    public override void OnInitialize()
    {
        base.OnInitialize();
        m_Characters = new Dictionary<int, ICharacter>();
    }

    public override void OnUpdate()
    {
        base.OnUpdate();
        //更新所有角色逻辑
        foreach (ICharacter ch in m_Characters.Values)
        {
            ch.OnUpdate();
        }
    }

    public override void OnRelease()
    {
        base.OnRelease();
        //删除所有角色
        foreach (ICharacter ch in m_Characters.Values)
        {
            ch.OnDestroy();
            GameObject.Destroy(ch.GameObject);
        }
        m_Characters.Clear();
    }

    //设置一个角色为可控制
    public void SetControlledCharacter(ICharacter ch)
    {
        //TODO 为上一个控制角色添加AI代理

        ch.FSM = Factory.Instance.CreateControlledAI(ch, m_RPGGame);
        m_ControlledCharacter = ch;
    }

    //添加一个角色
    public void AddCharacter(ICharacter ch)
    {
        m_Characters.Add(ch.InstanceId, ch);
    }

    //删除一个角色
    public void RemoveCharacter(ICharacter ch)
    {
        m_Characters.Remove(ch.InstanceId);
        ch.OnDestroy();
        GameObject.Destroy(ch.GameObject);
    }

    //创建玩家角色
    public ICharacter CreatePlayer()
    {
        ICharacter ch = Factory.Instance.CreateCharacter<Player>(1);
        WearDefaultEquipment(ch);
        Transform bornPoint = GameObject.FindWithTag("BornPoint").transform;
        ch.Transform.position = bornPoint.position;
        SetControlledCharacter(ch);
        m_RPGGame.GetCamera().FollowCharacter(ch);
        AddCharacter(ch);
        return ch;
    }

    //得到玩家角色
    public ICharacter GetPlayer()
    {
        return m_ControlledCharacter;
    }

    //创建NPC
    public ICharacter CreateNPC<T>(int id, Vector3 position, Quaternion rotation) where T : ICharacter, new()
    {
        ICharacter ch = Factory.Instance.CreateCharacter<T>(id);
        WearDefaultEquipment(ch);
        ch.Transform.position = position;
        ch.Transform.rotation = rotation;
        AddCharacter(ch);
        return ch;
    }

    //设置AI
    public void SetAI(ICharacter ch, int fsmId)
    {
        IFSMMachine fsm = Factory.Instance.CreateFSM(ch, fsmId);
        ch.FSM = fsm;
        if (fsm is FSMController)
        {
            (fsm as FSMController).SwitchToDefaultState();
        }
    }

    //得到一个指定方向扇形范围内的角色
    public ICharacter GetACharacterWithFanShape(ICharacter self, Vector3 dir, float angle, float distance)
    {
        ICharacter character = null;
        foreach (ICharacter ch in m_Characters.Values)
        {
            if (ch.InstanceId == self.InstanceId)
                continue;
            if (MathEx.DetectFanCollision(ch.Transform.position, self.Transform.position, dir, angle, distance))
            {
                character = ch;
                break;
            }
        }
        return character;
    }

    //得到指定方向扇形范围内的所有角色
    public List<ICharacter> GetCharactersWithFanShape(ICharacter self, Vector3 dir, float angle, float distance)
    {
        List<ICharacter> characters = new List<ICharacter>();
        foreach (ICharacter ch in m_Characters.Values)
        {
            if (ch.InstanceId == self.InstanceId)
                continue;
            if (MathEx.DetectFanCollision(ch.Transform.position, self.Transform.position, dir, angle, distance))
            {
                characters.Add(ch);
            }
        }
        return characters;
    }

    public override void OnRecMessage(WorldMessage msg)
    {
        base.OnRecMessage(msg);
        if (msg.MessageId == MessageDefines.MSG_CHS_ONINPUT)
        {
            if (m_ControlledCharacter != null)
            {
                m_ControlledCharacter.OnRecMessage(msg);
            }
        }
    }

    private void WearDefaultEquipment(ICharacter ch)
    {
        //默认装备
        for (int i = 0; i < ch.Config.DefaultEquipments.Length; ++i)
        {
            if (ch.Config.DefaultEquipments[i] != 0)
                ch.Wear((EPartOfBodyType)i, Factory.Instance.CreateEquipment(ch.Config.DefaultEquipments[i]));
        }
    }
}
