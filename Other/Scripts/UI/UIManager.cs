using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using UnityEngine;

public class UIManager : SingletonMono<UIManager>
{
    private Transform m_RootTrans;
    private GameObject m_RootGo;
    private Camera m_Camera;
    private Canvas m_Canvas;

    private List<IUserInterface> m_GroupUIs;
    private List<IUserInterface> m_SingleUIs;
    private List<IUserInterface> m_HUDs;

    private IUserInterface m_ActivatedUI;

    public Camera Camera
    {
        get {  return m_Camera;  }
    }

    protected override void Awake()
    {
        base.Awake();
        //获取重要组件
        m_RootTrans = transform;
        m_RootGo = gameObject;
        m_Camera = GetComponentInChildren<Camera>();
        m_Canvas = GetComponentInChildren<Canvas>();

        m_GroupUIs = new List<IUserInterface>();
        m_SingleUIs = new List<IUserInterface>();
        m_HUDs = new List<IUserInterface>();

        //注册监听按钮事件
        InputSystem.Instance.AddInputEvent(HandleInput, InputConst.INPUT_PRIORITY_UI);
    }

    protected override void OnDestroy()
    {
        base.OnDestroy();
        //取消监听按钮事件
        InputSystem.Instance.RemoveInputEvent(HandleInput);
    }

    private void Update()
    {
        m_GroupUIs.ForEach((ui) => { ui.OnUpdate(); });
        m_SingleUIs.ForEach((ui) => { ui.OnUpdate(); });
        m_HUDs.ForEach((ui) => { ui.OnUpdate(); });
    }

    //打开UI
    public T OpenUI<T>() where T : IUserInterface, new()
    {
        IUserInterface ui = new T();
        ui.Transform.SetParent(m_RootTrans);
        ui.Transform.localScale = Vector3.one;
        ui.Transform.offsetMin = Vector3.zero;
        ui.Transform.offsetMax = Vector3.zero;
        if (ui.Type == EUIType.GroupUI)
        {
            ui.Transform.SetAsLastSibling();
            m_GroupUIs.Add(ui);
            SetActivate(ui);
        }
        else if (ui.Type == EUIType.SingleUI)
        {
            m_SingleUIs.Add(ui);
        }
        else if (ui.Type == EUIType.HUD)
        {
            m_HUDs = new List<IUserInterface>();
        }
        Util.Invoke(ui, "OnInitialize");
        return ui as T;
    }

    //关闭UI
    public void CloseUI(IUserInterface ui)
    {
        if (ui.Type == EUIType.GroupUI)
        {
            m_GroupUIs.Remove(ui);
        }
        else if (ui.Type == EUIType.SingleUI)
        {
            m_SingleUIs.Remove(ui);
        }
        else if (ui.Type == EUIType.HUD)
        {
            m_HUDs.Remove(ui);
        }
        Util.Invoke(ui, "OnDestory");
        if (m_ActivatedUI == ui)
            m_ActivatedUI = null;
        GameObject.Destroy(ui.GameObject);
    }

    //关闭所有GroupUI
    public void CloseAllGroupUI()
    {
        while (m_GroupUIs.Count > 0)
        {
            CloseUI(m_GroupUIs[m_GroupUIs.Count - 1]);
        }
    }

    //设置一个UI为当前激活
    public void SetActivate(IUserInterface ui)
    {
        m_ActivatedUI = ui;
    }

    //处理按钮输入的回调
    public bool HandleInput(EInputWord word, bool down)
    {
        if (m_ActivatedUI != null)
            return m_ActivatedUI.OnInput(word, down);
        return false;
    }
}

public abstract class IUserInterface
{
    protected RectTransform m_Transform;
    protected GameObject m_GameObject;
    private EUIType m_Type;
    private Coroutine m_Coroutine;

    public EUIType Type { get { return m_Type; } }

    public GameObject GameObject { get { return m_GameObject; } }

    public RectTransform Transform { get { return m_Transform; } }

    public IUserInterface(string prefabName, EUIType type)
    {
        m_GameObject = GameObject.Instantiate(ResourceFactory.Instance.LoadUI(prefabName)) as GameObject;
        m_Transform = GameObject.GetComponent<RectTransform>();
        m_Type = type;
        m_Coroutine = UIManager.Instance.StartCoroutine(WaitForGameObjectInstantiate());
    }

    public virtual void OnUpdate() { }

    //处理输入
    public virtual bool OnInput(EInputWord word, bool down) { return false; }

    //在IUserInterface构造时调用
    protected virtual void OnInitialize() { }

    //在GameObject完全被加载后调用
    protected virtual void OnLoaded() { }

    //当IUserInterface被销毁时调用
    protected virtual void OnDestory()
    {
        UIManager.Instance.StopCoroutine(m_Coroutine);
    }

    private IEnumerator WaitForGameObjectInstantiate()
    {
        yield return null;
        OnLoaded();
    }
}

public enum EUIType
{
    SingleUI,
    GroupUI,
    HUD,
}