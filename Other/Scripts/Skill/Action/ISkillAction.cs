using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class ISkillAction
{
    private bool m_Begin;       //是否开始
    private bool m_End;         //是否结束
    protected Skill m_Owner;    //操作的持有者

    public bool Begin { get { return m_Begin; } set { m_Begin = value; } }

    public bool End
    {
        get
        {
            return m_End;
        }

        set
        {
            m_End = value;
        }
    }

    public ISkillAction(Skill owner, Object m_Params)
    {
        m_Owner = owner;
        UnpackParams(m_Params);
    }

    public abstract void Execute();

    //返回值决定该操作是否已经结束
    public virtual bool OnUpdate() { return true; }

    //终止
    public virtual void Break() { }

    //分析参数
    protected abstract void UnpackParams(Object _params);
}
