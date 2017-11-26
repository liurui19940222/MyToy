using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StartScene : BaseScene {

    public StartScene(int id) : base("StartScene", id) { }

    protected override void OnLoaded()
    {
        base.OnLoaded();

        //添加输入设备
        InputSystem.Instance.AddInputDevice(new XboxInput());
        InputSystem.Instance.AddInputDevice(new KeyboardInput());

        //切换到主菜单场景
        MessageProxy.SendSwitchSceneMsg(GameSceneManager.SCENE_ID_MAINMENU);
    }

    public override void OnExit()
    {
        base.OnExit();
    }

    public override int OnUpdate()
    {
        return base.OnUpdate();
    }
}
