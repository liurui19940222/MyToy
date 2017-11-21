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

        //创建主角
        CreatePlayer();
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
    private void CreatePlayer()
    {
        ICharacter ch = Factory.Instance.CreateCharacter<Player>("char_cfg_001");
        SetControlledCharacter(ch);
        m_RPGGame.GetCamera().FollowCharacter(ch);
        AddCharacter(ch);
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

}
