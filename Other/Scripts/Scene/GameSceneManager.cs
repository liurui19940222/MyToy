using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameSceneManager : FSMMachine {

    public const int SCENE_ID_START = 1;
    public const int SCENE_ID_MAINMENU = 2;
    public const int SCENE_ID_WORLD = 3;

    public override void OnInitialize()
    {
        AddState(new StartScene(SCENE_ID_START));
        AddState(new MainMenuScene(SCENE_ID_MAINMENU));
        AddState(new WorldScene(SCENE_ID_WORLD));
        SwitchScene(SCENE_ID_START);

        MessageCenter.Instance.AddListener(MessageDefines.MSG_SWITCH_SCENE, OnRecSwitchScene);
    }

    public override void OnUpdate()
    {
        InputSystem.Instance.UpdateInput();

        base.OnUpdate();
    }

    public override void Exit()
    {
        MessageCenter.Instance.RemoveListener(MessageDefines.MSG_SWITCH_SCENE, OnRecSwitchScene);
    }

    private void OnRecSwitchScene(IMessage msg)
    {
        SwitchScene((msg as MSGSwitchScene).SceneId);
    }

    public void SwitchScene(int sceneId)
    {
        SwitchToState(sceneId);
        (m_CurState as BaseScene).LoadScene();
    }
}
