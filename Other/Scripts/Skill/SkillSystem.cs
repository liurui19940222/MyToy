using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SkillSystem : IGameSystem
{
    private Dictionary<int, List<Skill>> m_ExecutingSkills;       //执行中的技能

    public SkillSystem(RPGGame game) : base(game)
    {
    }

    public override void OnInitialize()
    {
        base.OnInitialize();
        m_ExecutingSkills = new Dictionary<int, List<Skill>>();
    }

    public override void OnUpdate()
    {
        base.OnUpdate();
        //更新所有技能逻辑
        foreach (List<Skill> list in m_ExecutingSkills.Values)
        {
            for (int i = 0; i < list.Count; ++i)
            {
                if (list[i].OnUpdate())
                {
                    list.RemoveAt(i--);
                }
            }
        }
    }

    public override void OnRelease()
    {
        base.OnRelease();
        //终止所有正在执行的技能
        foreach (List<Skill> list in m_ExecutingSkills.Values)
        {
            for (int i = 0; i < list.Count; ++i)
            {
                if (list[i].OnUpdate())
                {
                    list[i].Break();
                }
            }
        }
        m_ExecutingSkills.Clear();
    }

    //执行一个技能
    public Skill Execute(ICharacter ch, int skillId)
    {
        SkillConfig config = ResourceFactory.Instance.LoadSkillConfig(skillId);
        //检查技能的执行条件
        for (int i = 0; i < config.Conditions.Count; ++i)
        {
            if (!ch.CheckSkillCondition(config.Conditions[i]))
                return null;
        }
        Skill skill = new Skill(this, ch, config);
        AddSkill(ch.InstanceId ,skill);
        return skill;
    }

    //判断一个角色是否所有技能都已经完成
    public bool HasDone(int characterId)
    {
        if (!m_ExecutingSkills.ContainsKey(characterId))
            return true;
        List<Skill> list = m_ExecutingSkills[characterId];
        for (int i = 0; i < list.Count; ++i)
        {
            if (!list[i].Done) return false;
        }
        return true;
    }

    //添加技能
    private void AddSkill(int characterId, Skill skill)
    {
        List<Skill> list = null;
        if (m_ExecutingSkills.ContainsKey(characterId))
        {
            list = m_ExecutingSkills[characterId];
        }
        else
        {
            list = new List<Skill>();
            m_ExecutingSkills.Add(characterId, list);
        }
        list.Add(skill);
    }
}
