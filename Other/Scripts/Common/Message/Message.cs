using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IMessage
{

}

public class Message : IMessage
{
    private Dictionary<string, object> m_Params;

    public Message() {
        m_Params = new Dictionary<string, object>();
    }

    public object this[string key]
    {
        set {
            if (m_Params.ContainsKey(key))
                m_Params[key] = value;
            else
                m_Params.Add(key, value);
        }
        get {
            if (m_Params.ContainsKey(key))
                return m_Params[key];
            return null;
        }
    }

    public void Reset() { m_Params.Clear(); }
}
