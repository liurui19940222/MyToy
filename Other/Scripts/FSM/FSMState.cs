using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FSMState
{

    private int m_Id;                               //状态Id
    private FSMMachine m_FSM;                       //持有该状态的状态机
    private bool m_IsHandledLastInput;              //是否处理了最后一次的输入消息，如果HandleInput返回非该状态Id，状态机会检查该标识，将输入转入下一个状态

    public int Id
    {
        get { return m_Id; }
        set { m_Id = value; }
    }

    public FSMMachine FSM
    {
        get { return m_FSM; }
        set { m_FSM = value; }
    }

    public bool IsHandledLastInput
    {
        get { return m_IsHandledLastInput; }
        set { m_IsHandledLastInput = value; }
    }

    public FSMState(int id) { m_Id = id; }

    public virtual int HandleInput(EFSMInputType input, Message msg)
    {
        IsHandledLastInput = true;
        return m_Id;
    }

    public virtual void OnEnter() { }

    public virtual void OnExit() { }

    public virtual int OnUpdate() { return m_Id; }

    public virtual bool HasAlready() { return true; }
}

