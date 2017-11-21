using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FSMMachine {

    private Dictionary<int, FSMState> m_States; //所有状态

    private FSMState m_DefaultState;            //默认状态

    protected FSMState m_CurState;              //当前状态

    public FSMMachine()
    {
        m_States = new Dictionary<int, FSMState>();
        OnEnter();
    }

    //添加一个状态
    public void AddState(FSMState state)
    {
        if (m_States.ContainsKey(state.Id))
            m_States[state.Id] = state;
        else
            m_States.Add(state.Id, state);
        state.FSM = this;
    }

    //获取状态
    public FSMState GetState(int id)
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
    public void SetAsDefaultState(FSMState state)
    {
        AddState(state);
        m_DefaultState = state;
    }

    //设置默认状态
    public void SetDefaultState(int id)
    {
        m_DefaultState = GetState(id);
    }

    //切换到一个状态
    public void SwitchToState(int id)
    {
        if (m_CurState != null && m_CurState.Id != id)
        {
            m_CurState.OnExit();
        }
        m_CurState = GetState(id);
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

    public void Exit()
    {
        OnExit();
    }

    protected virtual void OnEnter() { }

    protected virtual void OnExit() { }
}
