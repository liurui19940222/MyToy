using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public abstract class BaseScene : FSMState
{

    private bool m_bLoadingDone;

    private string m_SceneName;

    private Coroutine m_LoadingCoroutine;

    public BaseScene(string sceneName, int id) : base(id) { m_SceneName = sceneName; }

    public override void OnEnter()
    {
        base.OnEnter();
        m_bLoadingDone = false;
        InputSystem.Instance.AddInputEvent(OnInput, InputConst.INPUT_PRIORITY_WORLD);
    }

    public override void OnExit()
    {
        base.OnExit();
        m_bLoadingDone = false;
        InputSystem.Instance.RemoveInputEvent(OnInput);
        if (m_LoadingCoroutine != null)
            MonoScript.Instance.StopCoroutine(m_LoadingCoroutine);
    }

    public void LoadScene()
    {
        m_LoadingCoroutine = MonoScript.Instance.DoCoroutine(DoLoadScene(m_SceneName));
    }

    public override bool HasAlready()
    {
        return m_bLoadingDone;
    }

    private IEnumerator DoLoadScene(string sceneName)
    {
        yield return SceneManager.LoadSceneAsync(sceneName);
        m_bLoadingDone = true;
        OnLoaded();
    }

    protected virtual void OnLoaded() { }

    protected virtual bool OnInput(EInputWord word, bool down) { return true; }
}
