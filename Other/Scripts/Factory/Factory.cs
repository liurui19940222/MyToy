using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Factory : Singleton<Factory> {

    private CharacterFactory m_CharacterFactory;    //角色工厂
    private AIFactory m_AIFactory;                  //AI工厂
    private SkillFactory m_SkillFactory;            //技能工厂

    protected override void OnInitialize()
    {
        base.OnInitialize();
        m_CharacterFactory = new CharacterFactory();
        m_AIFactory = new AIFactory();
        m_SkillFactory = new SkillFactory();
    }

    //创建世界中的角色
    public T CreateCharacter<T>(string config) where T : ICharacter, new()
    {
        return m_CharacterFactory.CreateCharacter<T>(config);
    }

    //创建角色的身体
    public IPartOfBody[] CreateBody()
    {
        return m_CharacterFactory.CreateBody();
    }

    //创建受玩家操作的AI
    public CharacterFSMMachine CreateControlledAI(ICharacter ch, RPGGame game)
    {
        return m_AIFactory.CreateControlledAI(ch, game);
    }

    //创建技能Action
    public ISkillAction CreateSkillAction(ESkillActionType type, Skill owner, Object _params)
    {
        return m_SkillFactory.CreateAction(type, owner, _params);
    }
}
