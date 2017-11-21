using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MainMenuScene : BaseScene {

    private UIMainMenu m_MainMenuUI;

    public MainMenuScene(int id) : base("MainMenuScene", id) { }

    public override void OnEnter()
    {
        base.OnEnter();
        m_MainMenuUI = UIManager.Instance.OpenUI<UIMainMenu>();
        MessageProxy.SendSwitchSceneMsg(GameSceneManager.SCENE_ID_WORLD);
    }

    public override void OnExit()
    {
        base.OnExit();
        UIManager.Instance.CloseAllGroupUI();
    }

    protected override bool OnInput(EInputWord word, bool down)
    {
        return base.OnInput(word, down);
    }
}
