using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WorldScene : BaseScene {

    public WorldScene(int id) : base(GameConfig.Instance.MainSceneName, id) { }

    protected override void OnLoaded()
    {
        base.OnLoaded();
        Debug.Log("loaded");
        RPGGame.Instance.Initialize();
        RPGGame.Instance.CreatePlayer();
        RPGGame.Instance.CreateNPCAndSetAI<Enemy>(3, new Vector3(0, 0, 3), Quaternion.LookRotation(Vector3.back), 2);
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
