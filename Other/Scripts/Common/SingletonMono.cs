using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SingletonMono<T> : MonoBehaviour where T : SingletonMono<T>
{

    private static T _instance;
    public static T Instance {
        get {
            if (_instance == null)
            {
                GameObject go = GameObject.Find(typeof(T).Name);
                if (go == null)
                    go = new GameObject(typeof(T).Name);
                _instance = go.GetComponent<T>();
                if (_instance == null)
                    _instance = go.AddComponent<T>();
            }
            return _instance;
        }
    }

    protected virtual void Awake()
    {
        DontDestroyOnLoad(gameObject);
    }

    protected virtual void OnDestroy()
    {
        _instance = null;
    }
}
