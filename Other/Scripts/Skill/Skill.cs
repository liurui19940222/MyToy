using System;
using System.Collections.Generic;
using UnityEngine;

public class Skill
{
    private ICharacter m_Character;             //发起技能的角色
    private bool m_bDone;                       //是否已经结束
    private bool m_ReadyToNext;                 //是否已经能被取消，进入下一个动作
    private SkillConfig m_Config;               //技能配置
    private SkillSystem m_System;               //技能系统
    private float m_Timer;                      //局部时间
    private bool[] m_EventStates;               //各事件的触发状态
    private int m_Counter;                      //已经触发的事件计数
    private List<ISkillAction> m_Actions;       //技能操作

    public bool Done { get { return m_bDone; } set { m_bDone = value; } }

    public bool ReadyToNext { get { return m_ReadyToNext; } set { m_ReadyToNext = value; } }

    public ICharacter Character { get { return m_Character; } }

    public SkillSystem System { get { return m_System; } }

    public string Name { get { return m_Config.Name; } }

    public int Id { get { return m_Config.Id; } }

    public Skill(SkillSystem system, ICharacter owner, SkillConfig config)
    {
        m_Character = owner;
        m_System = system;
        m_Config = config;
        m_EventStates = new bool[m_Config.Events.Count];
        m_Actions = new List<ISkillAction>();
        for (int i = 0; i < m_Config.Events.Count; ++i)
        {
            m_Actions.Add(m_Config.Events[i].CreateAction(this));
        }

        if (m_Config.ToStiff)
        {
            owner.Stiff = true;
        }
    }

    public bool OnUpdate()
    {
        m_Timer += Time.deltaTime;
        //更新执行所有Event
        for (int i = 0; i < m_Config.Events.Count; ++i)
        {
            if (!m_EventStates[i] && m_Timer >= m_Config.Events[i].DelayTime)
            {
                if (!m_Character.CheckSkillCondition(m_Config.Events[i].Condition))
                {
                    if (m_Config.Events[i].BreakSkillWhenConditionFail)
                    {
                        Break();
                        return true;
                    }
                }
                else
                {
                    m_Actions[i].Begin = true;
                    m_Actions[i].Execute();
                    m_EventStates[i] = true;
                    m_Counter++;
                }
            }
        }

        //更新执行中的Action
        for (int i = 0; i < m_Actions.Count; ++i)
        {
            if (!m_Actions[i].Begin || m_Actions[i].End)
                continue;
            //如果Action返回true，则删除
            if (m_Actions[i].OnUpdate())
            {
                m_Actions[i].End = true;
            }
        }

        //如果执行完所有Action和Event，返回true移除该技能
        if (m_Actions.Count == 0 && m_Counter >= m_EventStates.Length)
            return true;

        return false;
    }

    //终止技能
    public void Break()
    {
        for (int i = 0; i < m_Actions.Count; ++i)
        {
            m_Actions[i].Break();
        }
        for (int i = 0; i < m_EventStates.Length; ++i)
        {
            m_EventStates[i] = false;
        }
        m_Timer = 0;
        m_bDone = true;
    }

    //根据输入键位获得下一击技能Id
    public int GetTargetId(EInputWord word)
    {
        for (int i = 0; i < m_Config.Transitions.Count; ++i)
        {
            if (m_Config.Transitions[i].Input == word)
                return m_Config.Transitions[i].TargetSkillId;
        }
        return 0;
    }
}

