using System;
using System.Collections.Generic;
using UnityEngine;

public class Skill
{
    private const int TRIGGER_EVENT_RET_SUCCESSFUL = 0; //触发事件成功
    private const int TRIGGER_EVENT_RET_FAILED = 1;     //触发事件失败
    private const int TRIGGER_EVENT_RET_BREAK = 2;      //触发事件中断（中断整个技能）

    private ICharacter m_Character;             //发起技能的角色
    private bool m_bDone;                       //是否已经结束
    private bool m_ReadyToNext;                 //是否已经能被取消，进入下一个动作
    private SkillConfig m_Config;               //技能配置
    private SkillSystem m_System;               //技能系统
    private float m_Timer;                      //局部时间
    private bool[] m_EventStates;               //各事件的触发状态
    private int m_EventCounter;                 //已经触发的事件计数
    private int m_ActionCounter;                //已经触发的Action计数
    private List<ISkillAction> m_Actions;       //技能操作
    private IEquipment m_ByWhichEquip;          //被哪件装备使用的
    private ICharacter m_TargetCharacter;       //技能目标（如果有）

    public bool Done { get { return m_bDone; } set { m_bDone = value; } }

    public bool ReadyToNext { get { return m_ReadyToNext; } set { m_ReadyToNext = value; } }

    public ICharacter Character { get { return m_Character; } }

    public SkillSystem System { get { return m_System; } }

    public string Name { get { return m_Config.Name; } }

    public int Id { get { return m_Config.Id; } }

    public IEquipment ByWhichEquip { get { return m_ByWhichEquip; } }

    public ICharacter TargetCharacter { get { return m_TargetCharacter; } }

    public Skill(SkillSystem system, ICharacter owner, SkillConfig config, IEquipment byWhichEquip, ICharacter target)
    {
        m_ByWhichEquip = byWhichEquip;
        m_TargetCharacter = target;
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
            owner.IsSkillStiff = true;
        }
    }

    public bool OnUpdate()
    {
        //如果执行完所有Action和Event，返回true移除该技能
        if (m_bDone && m_ActionCounter >= m_Actions.Count && m_EventCounter >= m_EventStates.Length)
            return true;

        m_Timer += Time.deltaTime;
        //更新执行所有Event
        for (int i = 0; i < m_Config.Events.Count; ++i)
        {
            //如果该事件没有被触发，并且是根据时间线来触发
            if (!m_EventStates[i] && m_Config.Events[i].TriggerType == SkillEvent.ETriggerType.TimeLine && m_Timer >= m_Config.Events[i].DelayTime)
            {
                if (TriggerEvent(i) == TRIGGER_EVENT_RET_BREAK)
                    return true;
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
                m_ActionCounter++;
            }
        }

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

    //接收一个消息
    public void OnRecGameMsg(Message msg)
    {
        if (m_bDone || m_EventCounter >= m_Config.Events.Count)
            return;
        ESkillMessageType type = (ESkillMessageType)msg["type"];
        if (type == ESkillMessageType.AnimationEvent)
        {
            for (int i = 0; i < m_Config.Events.Count; ++i)
            {
                if (!m_EventStates[i] && m_Config.Events[i].TriggerType == SkillEvent.ETriggerType.AnimEvent && m_Config.Events[i].AnimEvent == (string)msg["name"])
                {
                    if (TriggerEvent(i) == TRIGGER_EVENT_RET_BREAK)
                        m_bDone = true;
                }
            }
        }
    }

    //触发一个事件
    private int TriggerEvent(int index)
    {
        if (!m_Character.CheckSkillCondition(m_Config.Events[index].Condition))
        {
            if (m_Config.Events[index].BreakSkillWhenConditionFail)
            {
                Break();
                return TRIGGER_EVENT_RET_BREAK;
            }
        }
        else
        {
            m_Actions[index].Begin = true;
            m_Actions[index].Execute();
            m_EventStates[index] = true;
            m_EventCounter++;
            return TRIGGER_EVENT_RET_SUCCESSFUL;
        }
        return TRIGGER_EVENT_RET_FAILED;
    }
}

