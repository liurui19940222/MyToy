using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameLoop : MonoBehaviour
{

    private GameSceneManager m_SceneManager;

    void Awake()
    {
        m_SceneManager = new GameSceneManager();
        DontDestroyOnLoad(gameObject);
    }

    void Update()
    {
        m_SceneManager.OnUpdate();
    }

    void OnDestroy()
    {
        m_SceneManager.Exit();
    }
}
