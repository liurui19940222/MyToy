using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FSMMachine : IFSMMachine {

    private Dictionary<int, FSMState> m_States; //所有状态

    private FSMState m_DefaultState;            //默认状态

    protected FSMState m_CurState;              //当前状态

    public FSMMachine()
    {
        m_States = new Dictionary<int, FSMState>();
        OnInitialize();
    }

    //添加一个状态
    public void AddState(IFSMState state)
    {
        if (m_States.ContainsKey(state.GetId()))
            m_States[state.GetId()] = state as FSMState;
        else
            m_States.Add(state.GetId(), state as FSMState);
        state.SetControllder(this);
    }

    //获取状态
    public IFSMState GetState(int id)
    {
        if (m_States.ContainsKey(id))
            return m_States[id];
        return null;
    }

    //删除状态
    public void RemoveState(int id)
    {
        if (m_States.ContainsKey(id))
            m_States.Remove(id);
    }

    //添加并置为默认状态
    public void SetAsDefaultState(IFSMState state)
    {
        AddState(state);
        m_DefaultState = state as FSMState;
    }

    //设置默认状态
    public void SetDefaultState(int id)
    {
        m_DefaultState = GetState(id) as FSMState;
    }

    //切换到一个状态
    public void SwitchToState(int id)
    {
        if (m_CurState != null && m_CurState.Id != id)
        {
            m_CurState.OnExit();
        }
        m_CurState = GetState(id) as FSMState;
        m_CurState.OnEnter();
    }

    //处理外部的输入
    public void HandleInput(EFSMInputType input, Message msg)
    {
        if (m_CurState != null)
        {
            int targetId = m_CurState.HandleInput(input, msg);
            if (targetId != m_CurState.Id)
            {
                bool isHandled = m_CurState.IsHandledLastInput;
                SwitchToState(targetId);
                //如果上一个状态未处理该消息，则传入至新状态继续处理
                if (!isHandled)
                {
                    m_CurState.HandleInput(input, msg);
                }
            }
        }
    }

    public virtual void OnUpdate()
    {
        if (m_CurState == null)
        {
            m_CurState = m_DefaultState;
            m_CurState.OnEnter();
        }

        //更新已经准备好的当前状态
        if (m_CurState != null && m_CurState.HasAlready())
        {
            int targetId = m_CurState.OnUpdate();
            if (targetId != m_CurState.Id)
            {
                SwitchToState(targetId);
            }
        }
    }

    public virtual void Exit()
    {
    }

    public virtual void OnInitialize()
    {
        
    }
}
