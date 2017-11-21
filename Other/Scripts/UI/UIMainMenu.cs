using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using DG.Tweening;
#if UNITY_EDITOR
using UnityEditor;
#endif

public class UIMainMenu : IUserInterface
{
    private const int SELECTION_NEWGAME = 0;    //新游戏
    private const int SELECTION_LOADGAME = 1;   //载入游戏
    private const int SELECTION_OPTION = 2;     //选项
    private const int SELECTION_QUIT = 3;       //退出游戏

    //所有选项的Transform
    private Transform[] m_SelcetionTrans;

    //光标
    private Transform m_Cursor;

    //光标初始的位置
    private Vector3 m_CursorRawPos;

    //选项之间的间隔
    private float m_IntervalHeight;

    //上一次选中的项
    private int m_LastSelection;

    //当前选中的项
    private int m_CurSelection;

    //选择时的音效
    private AudioClip m_SelectSound;

    public UIMainMenu() : base("UIMainMenu", EUIType.GroupUI)
    {

    }

    public override bool OnInput(EInputWord word, bool down)
    {
        //不处理按键抬起消息
        if (!down) return false;

        //根据按钮上下选择
        if (word == EInputWord.DPAD_UP)
            SetSelection(--m_CurSelection);
        else if (word == EInputWord.DPAD_DOWN)
            SetSelection(++m_CurSelection);

        //确认键处理
        if (word == EInputWord.A || word == EInputWord.START)
        {
            if (m_CurSelection == SELECTION_NEWGAME)
            {
                MessageProxy.SendSwitchSceneMsg(GameSceneManager.SCENE_ID_WORLD);
            }
            else if (m_CurSelection == SELECTION_QUIT)
            {
#if UNITY_EDITOR
                EditorApplication.isPlaying = false;
#else
                Application.Quit();
#endif
            }
        }

        //不再继续向下派发消息
        return true;
    }

    protected override void OnInitialize()
    {
        base.OnInitialize();
        //获取组件
        m_SelcetionTrans = new Transform[4];
        Transform selectionTrans = m_Transform.FindChild("selection");
        for (int i = 0; i < selectionTrans.childCount; ++i)
        {
            m_SelcetionTrans[i] = selectionTrans.GetChild(i);
        }
        m_Cursor = m_Transform.FindChild("cursor");
        m_IntervalHeight = m_SelcetionTrans[0].transform.position.y - m_SelcetionTrans[1].transform.position.y;
        //计算位置信息
        m_CursorRawPos = m_Cursor.position;
        m_SelectSound = ResourceFactory.Instance.LoadSound("menu_select");
        //默认选中第0个
        SetSelection(0, true);
    }

    //设置选中
    private void SetSelection(int index, bool first = false)
    {
        int i = Mathf.Clamp(index, 0, m_SelcetionTrans.Length - 1);
        if (i == index)
        {
            m_Cursor.position = m_CursorRawPos + Vector3.down * (i * m_IntervalHeight);
            m_SelcetionTrans[index].DOScale(1.15f, first ? 0.0f : 0.25f);
            if (!first)
            {
                SoundManager.Instance.PlaySound(m_SelectSound);
                m_SelcetionTrans[m_LastSelection].DOScale(1.0f, 0.25f);
            }
        }
        m_CurSelection = i;
        m_LastSelection = i;
    }
}
