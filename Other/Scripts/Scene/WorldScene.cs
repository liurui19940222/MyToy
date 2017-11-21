using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WorldScene : BaseScene {

    public WorldScene(int id) : base("WorldScene", id) { }

    public override void OnEnter()
    {
        base.OnEnter();
    }

    protected override void OnLoaded()
    {
        base.OnLoaded();
        RPGGame.Instance.Initialize();
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
