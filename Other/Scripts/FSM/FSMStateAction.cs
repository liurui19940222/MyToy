using UnityEngine;
using System.Reflection;
using System.Collections.Generic;

[System.Serializable]
public class FSMStateAction : IFSMState
{
    [SerializeField]
    private string m_StateName;

    [SerializeField]
    private int m_Id;

    [SerializeField]
    private FSMController m_Controller;

    [SerializeField]
    private List<FSMTransition> m_Transitions;

    [SerializeField]
    private object m_Agent;

    [SerializeField]
    private string m_OnEnterMethod;

    [SerializeField]
    private string m_OnExitMethod;

    [SerializeField]
    private string m_OnUpdateMethod;

    private MethodInfo m_OnEnter;
    private MethodInfo m_OnExit;
    private MethodInfo m_OnUpdate;

    public List<FSMTransition> Transitions
    {
        get { return m_Transitions; }
        set { m_Transitions = value; }
    }

    public string StateName
    {
        get { return m_StateName; }
        set { m_StateName = value; }
    }

    public object Agent
    {
        get { return m_Agent; }
        set { m_Agent = value; }
    }

    public string OnEnterMethod
    {
        get { return m_OnEnterMethod; }
        set { m_OnEnterMethod = value; }
    }

    public string OnExitMethod
    {
        get { return m_OnExitMethod; }
        set { m_OnExitMethod = value; }
    }

    public string OnUpdateMethod
    {
        get { return m_OnUpdateMethod; }
        set { m_OnUpdateMethod = value; }
    }

    public IFSMMachine GetController()
    {
        return m_Controller;
    }

    public void SetControllder(IFSMMachine fsm)
    {
        m_Controller = fsm as FSMController;
    }

    public int GetId()
    {
        return m_Id;
    }

    public int HandleInput(EFSMInputType input, Message msg)
    {
        if (input == EFSMInputType.FSMCondition)
        {
            List<FSMParameter> _params = (List<FSMParameter>)msg["params"];
            FSMTransition transition = null;
            FSMCondition condition = null;
            FSMParameter parameter = null;
            bool checkSuccess = false;
            for (int i = 0; i < m_Transitions.Count; ++i)   //遍历向各目标状态的转换
            {
                transition = m_Transitions[i];
                checkSuccess = true;
                for (int j = 0; j < transition.Conditions.Count; ++j)   //遍历转换的各个条件
                {
                    condition = transition.Conditions[j];
                    for (int k = 0; k < _params.Count; ++k)
                    {
                        parameter = _params[k];
                        if (condition.Name == parameter.Name)
                        {
                            checkSuccess = checkSuccess && condition.CheckParamater(parameter); //与状态机的当前参数值作检查
                            if (!checkSuccess)
                                break;
                        }
                    }
                    if (!checkSuccess)
                        break;
                }
                if (checkSuccess)
                    return transition.TargetStateId;
            }
        }
        return -1;
    }

    public bool HasAlready()
    {
        return true;
    }

    public void OnEnter()
    {
        if (m_Agent != null)
        {
            System.Type type = m_Agent.GetType();
            if (m_OnEnter == null)
                m_OnEnter = type.GetMethod(m_OnEnterMethod, BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance);
            if (m_OnExit == null)
                m_OnExit = type.GetMethod(m_OnExitMethod, BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance);
            if (m_OnUpdate == null)
                m_OnUpdate = type.GetMethod(m_OnUpdateMethod, BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance);
            if (m_OnEnter != null)
                m_OnEnter.Invoke(m_Agent, null);
        }
        Debug.Log(m_OnEnterMethod);
    }

    public void OnExit()
    {
        if (m_Agent != null && m_OnExit != null)
            m_OnExit.Invoke(m_Agent, null);
        Debug.Log(m_OnExitMethod);
    }

    public int OnUpdate()
    {
        if (m_Agent != null && m_OnUpdate != null)
        {
            object obj = m_OnUpdate.Invoke(m_Agent, null);
            return System.Convert.ToInt32(obj);
        }
        return m_Id;
    }

    public void AddTransition(FSMTransition transition)
    {
        Transitions.Add(transition);
    }

    public void DeleteTransition(FSMTransition transition)
    {
        Transitions.Remove(transition);
    }
}
