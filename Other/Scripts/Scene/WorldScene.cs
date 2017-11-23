using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WorldScene : BaseScene {

    public WorldScene(int id) : base(GameConfig.Instance.MainSceneName, id) { }

    public override void OnEnter()
    {
        base.OnEnter();
    }

    protected override void OnLoaded()
    {
        base.OnLoaded();
        RPGGame.Instance.Initialize();
        RPGGame.Instance.CreatePlayer();
        RPGGame.Instance.CreateNPC(2, new Vector3(0, 0, 3), Quaternion.LookRotation(Vector3.back));
    }

    public override int OnUpdate()
    {
        RPGGame.Instance.OnUpdate();
        return base.OnUpdate();
    }

    public override void OnExit()
    {
        base.OnExit();
        RPGGame.Instance.Quit();
    }
}
