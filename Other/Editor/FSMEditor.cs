using System;
using System.Collections;
using System.Reflection;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEditorInternal;
using System.Security.Policy;

public class FSMEditor : EditorWindow
{
    private FSMController m_Controller;
    private Rect m_LeftWindowRect;
    private ReorderableList m_ParamsList;
    private ReorderableList m_ConditionList;
    private Dictionary<int, FSMStateWindow> m_StateWindows = new Dictionary<int, FSMStateWindow>();
    private int m_SelectedState;
    private Type m_AgentType;
    private List<string> m_MethodNames;
    private Texture m_BgTexture;
    private RightViewType m_VeiwType = RightViewType.State;

    EditorArrow m_Arrow = new EditorArrow();

    public Type AgentType
    {
        get { return m_AgentType; }
        set { m_AgentType = value; }
    }

    public List<string> MethodNames
    {
        get { return m_MethodNames; }
        set { m_MethodNames = value; }
    }

    public Texture BgTexture
    {
        get { return m_BgTexture; }
        set { m_BgTexture = value; }
    }

    private Vector2 m_MouseLastPos;

    [MenuItem("Tools/FSM Editor")]
    public static void OpenWindow()
    {
        GetWindow<FSMEditor>().Show();
    }

    void OnEnable()
    {
        if (BgTexture == null)
            BgTexture = Resources.Load<Texture2D>("white");
        m_StateWindows.Clear();
        minSize = new Vector2(780, 376);
    }

    void OnGUI()
    {
        Color color = GUI.color;
        GUI.color = Color.grey;
        GUI.Box(new Rect(0, 0, position.width, position.height), string.Empty);
        GUI.color = color;

        if (m_BeginToMakeTransition)
        {
            Repaint();
            m_Arrow.EndPos = Event.current.mousePosition;
            m_Arrow.Draw(Color.white);

            if (EditorTools.IsMouseDown(1))
            {
                m_BeginToMakeTransition = false;
            }
        }

        if (EditorTools.IsMouseDown(0))
        {
            foreach (FSMStateWindow window in m_StateWindows.Values)
            {
                window.UpdateCollision();
            }
        }

        foreach (FSMStateWindow window in m_StateWindows.Values)
        {
            window.DrawArrow();
        }

        BeginWindows();

        //绘制左边参数窗口
        m_LeftWindowRect.Set(0, 0, 200, position.height);
        m_LeftWindowRect = GUI.Window(999, m_LeftWindowRect, OnDrawLeftWindow, "");

        //绘制右边窗口
        Rect centerRect = new Rect(position.width - 250, 0, 250, position.height);
        GUI.Window(1000, centerRect, OnDrawRightWindow, "");


        //绘制状态
        if (m_Controller != null)
        {
            if (m_StateWindows.Count != m_Controller.States.Count)
            {
                m_StateWindows.Clear();
                FSMStateAction state = null;
                for (int i = 0; i < m_Controller.States.Count; ++i)
                {
                    state = m_Controller.States[i];
                    FSMStateWindow window = new FSMStateWindow();
                    window.m_Controller = m_Controller;
                    window.m_StateId = state.GetId();
                    window.m_WindowId = state.GetId();
                    window.m_Editor = this;
                    m_StateWindows.Add(state.GetId(), window);
                }
            }
            foreach (FSMStateWindow window in m_StateWindows.Values)
            {
                window.Draw();
            }
        }

        //右键菜单
        if (m_Controller != null && !m_BeginToMakeTransition)
        {
            if (EditorTools.IsMouseDown(1))
            {
                List<string> list = new List<string>();
                list.Add("Create State");
                m_MouseLastPos = Event.current.mousePosition;
                EditorTools.CreateMenuWithTextList(list, OnContextMenu);
            }
        }

        //删除当前选中的条件
        if (Event.current.isKey && Event.current.keyCode == KeyCode.Delete && Event.current.type == EventType.KeyDown)
        {
            if (m_SelectedTransition != null)
            {
                FSMStateAction state = m_Controller.GetState(m_SelectedTransitionState) as FSMStateAction;
                if (state != null)
                {
                    state.DeleteTransition(m_SelectedTransition);
                    m_SelectedTransitionState = -1;
                    m_SelectedTransition = null;
                    Repaint();
                }
            }
        }


        EndWindows();
    }

    //删除一个状态窗口
    public void DeleteWindow(int id)
    {
        m_StateWindows.Remove(id);
    }

    //设置选中状态id
    public void SetSelectedState(int id)
    {
        m_SelectedState = id;
        m_VeiwType = RightViewType.State;
        m_SelectedArrow = null;
    }

    //开始准备一个连线
    public bool m_BeginToMakeTransition = false;
    private int m_SrcState;
    public void BeginMakeATransition(Vector2 startPos, int srcState)
    {
        m_SrcState = srcState;
        m_Arrow.StartPos = startPos;
        m_BeginToMakeTransition = true;
    }

    //完成一个连线
    public void EndTransition(int dstState)
    {
        FSMStateAction state = m_Controller.GetState(m_SrcState) as FSMStateAction;
        FSMTransition transition = new FSMTransition();
        transition.TargetStateId = dstState;
        state.AddTransition(transition);
        m_BeginToMakeTransition = false;
    }

    //选中一个连线
    public EditorArrow m_SelectedArrow;
    private FSMTransition m_SelectedTransition;
    private int m_SelectedTransitionState;
    public void SelectTransition(EditorArrow arrow, int state, FSMTransition transition)
    {
        m_SelectedArrow = arrow;
        m_SelectedTransition = transition;
        m_SelectedTransitionState = state;
        m_VeiwType = RightViewType.Transition;
        CreateConditionList();
    }

    //重绘
    public void RepaintView()
    {
        Repaint();
    }

    //监听上下文菜单事件
    void OnContextMenu(int index)
    {
        if (index == 0)
        {
            FSMStateAction state = new FSMStateAction();
            state.Id = m_Controller.StateIdSequence++;
            state.StateName = "New State";
            state.Rect = new Rect(m_MouseLastPos.x, m_MouseLastPos.y, 150, 55);
            state.SetControllder(m_Controller);
            m_Controller.AddState(state);
        }
    }

    //绘制参数窗口
    void OnDrawLeftWindow(int windowId)
    {
        EditorGUILayout.BeginHorizontal();

        GUILayout.Label("拖放");
        m_Controller = EditorGUILayout.ObjectField(m_Controller, typeof(FSMController), true) as FSMController;

        EditorGUILayout.EndHorizontal();

        if (m_Controller == null)
            return;

        EditorGUILayout.BeginHorizontal();

        GUILayout.Label("代理");
        List<Type> typeList = GetFSMCallClassList();
        string[] textList = new string[typeList.Count];
        int selected = -1;
        for (int i = 0; i < typeList.Count; ++i)
        {
            if (m_Controller.AgentType == typeList[i].ToString())
            {
                selected = i;
            }
            textList[i] = typeList[i].ToString();
        }
        selected = EditorGUILayout.Popup(selected, textList);
        if (selected != -1)
        {
            AgentType = typeList[selected];
            m_Controller.AgentType = typeList[selected].ToString();
            MethodInfo[] methods = AgentType.GetMethods(BindingFlags.Public | BindingFlags.Instance | BindingFlags.IgnoreCase);
            m_MethodNames = new List<string>();
            m_MethodNames.Add("None");
            for (int i = 0; i < methods.Length; ++i)
            {
                if (IsAttribute(methods[i]))
                {
                    m_MethodNames.Add(methods[i].Name);
                }
            }
        }

        EditorGUILayout.EndHorizontal();

        if (m_ParamsList == null)
            CreateParamsList();
        m_ParamsList.DoLayoutList();
    }

    //绘制右边窗口
    void OnDrawRightWindow(int windowId)
    {
        if (m_Controller == null)
            return;
        if (m_VeiwType == RightViewType.State)
        {
            FSMStateAction state = m_Controller.GetState(m_SelectedState) as FSMStateAction;
            if (state == null)
                return;

            EditorGUILayout.BeginHorizontal();
            EditorGUILayout.LabelField("Id", GUILayout.Width(50));
            EditorGUILayout.LabelField(state.Id.ToString());
            EditorGUILayout.EndHorizontal();

            EditorGUILayout.BeginHorizontal();
            EditorGUILayout.LabelField("名字", GUILayout.Width(50));
            state.StateName = EditorGUILayout.TextField(state.StateName);
            EditorGUILayout.EndHorizontal();

            EditorGUILayout.BeginHorizontal();
            EditorGUILayout.LabelField("进入时", GUILayout.Width(50));
            int index = GetMethodIndexInArray(state.OnEnterMethod);
            index = EditorGUILayout.Popup(index, m_MethodNames.ToArray());
            state.OnEnterMethod = m_MethodNames[index] == "None" ? string.Empty : m_MethodNames[index];
            EditorGUILayout.EndHorizontal();

            EditorGUILayout.BeginHorizontal();
            EditorGUILayout.LabelField("更新时", GUILayout.Width(50));
            index = GetMethodIndexInArray(state.OnUpdateMethod);
            index = EditorGUILayout.Popup(index, m_MethodNames.ToArray());
            state.OnUpdateMethod = m_MethodNames[index] == "None" ? string.Empty : m_MethodNames[index];
            EditorGUILayout.EndHorizontal();

            EditorGUILayout.BeginHorizontal();
            EditorGUILayout.LabelField("退出时", GUILayout.Width(50));
            index = GetMethodIndexInArray(state.OnExitMethod);
            index = EditorGUILayout.Popup(index, m_MethodNames.ToArray());
            state.OnExitMethod = m_MethodNames[index] == "None" ? string.Empty : m_MethodNames[index];
            EditorGUILayout.EndHorizontal();
        }
        else if (m_VeiwType == RightViewType.Transition)
        {
            if (m_SelectedTransition != null)
            {
                FSMStateAction fromState = m_Controller.GetState(m_SelectedTransitionState) as FSMStateAction;
                FSMStateAction toState = m_Controller.GetState(m_SelectedTransition.TargetStateId) as FSMStateAction;
                if (fromState == null || toState == null)
                    return;
                EditorGUILayout.LabelField(string.Format("{0}----->{1}", fromState.StateName, toState.StateName));
                if (m_ConditionList == null)
                    CreateConditionList();
                m_ConditionList.DoLayoutList();
            }
        }
    }

    int GetMethodIndexInArray(string name)
    {
        int index = 0;
        for (int i = 0; i < m_MethodNames.Count; ++i)
        {
            if (m_MethodNames[i] == name)
            {
                index = i;
                break;
            }
        }
        return index;
    }

    //创建参数列表
    void CreateParamsList()
    {
        m_ParamsList = new ReorderableList(m_Controller.Paramaters, typeof(FSMParameter));

        m_ParamsList.drawHeaderCallback = (rect) =>
        {
            EditorGUI.LabelField(rect, "Parameters");
        };

        m_ParamsList.drawElementCallback = (rect, index, isActive, isFocused) =>
        {
            rect.height -= 4;
            rect.y += 2;
            List<float> weights = new List<float>();
            weights.Add(0.7f);
            weights.Add(0.05f);
            weights.Add(0.25f);
            List<Rect> rects = Util.SplitRect(rect, weights);
            m_Controller.Paramaters[index].Name = EditorGUI.TextField(rects[0], m_Controller.Paramaters[index].Name);
            ConditionValue value = m_Controller.Paramaters[index].Value;
            if (value.Type == EValueType.Bool)
            {
                value.boolValue = EditorGUI.Toggle(rects[2], value.boolValue);
            }
            else if (value.Type == EValueType.Float)
            {
                value.floatValue = EditorGUI.FloatField(rects[2], value.floatValue);
            }
            else if (value.Type == EValueType.Int)
            {
                value.intValue = EditorGUI.IntField(rects[2], value.intValue);
            }
            else if (value.Type == EValueType.Trigger)
            {
                value.triggerValue = EditorGUI.Toggle(rects[2], value.triggerValue);
            }
        };

        m_ParamsList.onAddCallback += (list) =>
        {
            EditorTools.CreateMenuWithEnum<EValueType>(typeof(EValueType), (index) =>
            {
                FSMParameter parameter = new FSMParameter();
                parameter.Name = "New " + (EValueType)index;
                parameter.Value = new ConditionValue();
                parameter.Value.Type = (EValueType)index;
                m_Controller.AddParamater(parameter);
            });
        };
    }

    //创建条件列表
    void CreateConditionList()
    {
        if (m_SelectedTransition.Conditions == null)
            m_SelectedTransition.Conditions = new List<FSMCondition>();
        m_ConditionList = new ReorderableList(m_SelectedTransition.Conditions, typeof(FSMCondition));

        m_ConditionList.drawHeaderCallback = (rect) =>
        {
            EditorGUI.LabelField(rect, "Conditions");
        };

        m_ConditionList.drawElementCallback = (rect, index, isActive, isFocused) =>
        {
            rect.height -= 4;
            rect.y += 2;
            if (m_SelectedTransition.Conditions == null || m_SelectedTransition.Conditions.Count <= index)
                return;
            FSMCondition condition = m_SelectedTransition.Conditions[index];
            if (condition == null)
                return;
            List<float> weights = new List<float>();
            weights.Add(0.475f);
            weights.Add(0.05f);
            weights.Add(0.475f);
            List<Rect> rects = Util.SplitRect(rect, weights);
            string[] paramtext = new string[m_Controller.Paramaters.Count];
            int selectedIndex = 0;
            for (int i = 0; i < m_Controller.Paramaters.Count; ++i)
            {
                paramtext[i] = m_Controller.Paramaters[i].Name;
                if (condition.Name == paramtext[i])
                {
                    selectedIndex = i;
                }
            }
            int newSelectedIndex = EditorGUI.Popup(rects[0], selectedIndex, paramtext);
            if (newSelectedIndex != selectedIndex)
            {
                condition.Name = m_Controller.Paramaters[newSelectedIndex].Name;
                condition.Value = new ConditionValue(m_Controller.Paramaters[newSelectedIndex].Value);
                condition.SetToDefaultType();
            }

            switch (condition.Value.Type)
            {
                case EValueType.Int:
                case EValueType.Float:
                    {
                        weights.Clear();
                        weights.Add(0.6f);
                        weights.Add(0.05f);
                        weights.Add(0.395f);
                        rects = Util.SplitRect(rects[2], weights);
                        string[] strs = { "Greater", "Less" };
                        int i = condition.Type == EConditionType.Greater ? 0 : 1;
                        i = EditorGUI.Popup(rects[0], i, strs);
                        condition.Type = i == 0 ? EConditionType.Greater : EConditionType.Less;
                        if (condition.Value.Type == EValueType.Int)
                            condition.Value.intValue = EditorGUI.IntField(rects[2], condition.Value.intValue);
                        else if (condition.Value.Type == EValueType.Float)
                            condition.Value.floatValue = EditorGUI.FloatField(rects[2], condition.Value.floatValue);
                    }
                    break;
                case EValueType.Bool:
                    {
                        string[] strs = { "false", "true" };
                        int i = condition.Value.boolValue ? 1 : 0;
                        i = EditorGUI.Popup(rects[2], i, strs);
                        condition.Value.boolValue = i == 0 ? false : true;
                    }
                    break;
                case EValueType.Trigger:
                    break;
            }
        };

        m_ConditionList.onAddCallback += (list) =>
        {
            if (m_Controller.Paramaters.Count == 0)
                return;
            if (m_SelectedTransition.Conditions == null)
                m_SelectedTransition.Conditions = new List<FSMCondition>();
            FSMCondition condition = new FSMCondition();
            condition.Name = m_Controller.Paramaters[0].Name;
            condition.Value = new ConditionValue(m_Controller.Paramaters[0].Value);
            condition.SetToDefaultType();
            m_SelectedTransition.Conditions.Add(condition);
        };
    }

    Color m_Color;

    void SetColor(Color newColor)
    {
        m_Color = GUI.color;
        GUI.color = newColor;
    }

    void ResetColor()
    {
        GUI.color = m_Color;
    }

    List<Type> GetFSMCallClassList()
    {
        List<Type> classList = new List<Type>();
        Assembly assembly = Assembly.Load("Assembly-CSharp");
        System.Type[] types = assembly.GetTypes();
        foreach (System.Type type in types)
        {
            System.Attribute[] attrs = System.Attribute.GetCustomAttributes(type, true);
            foreach (System.Attribute attr in attrs)
            {
                if (attr is FSMCallClass)
                {
                    classList.Add(type);
                    break;
                }
            }
        }
        return classList;
    }

    bool IsAttribute(MethodInfo method)
    {
        object[] attrs = method.GetCustomAttributes(typeof(FSMCallMethod), true);
        foreach (object attr in attrs)
        {
            if (attr is FSMCallMethod)
            {
                return true;
            }
        }
        return false;
    }

    enum RightViewType
    {
        State,
        Transition,
    }
}

public class FSMStateWindow
{
    public int m_WindowId;
    public FSMController m_Controller;
    public int m_StateId;
    public FSMEditor m_Editor;
    public List<EditorArrow> m_Arrows = new List<EditorArrow>();

    public void Draw()
    {
        FSMStateAction state = m_Controller.GetState(m_StateId) as FSMStateAction;
        if (state != null)
            state.Rect = GUI.Window(m_WindowId, state.Rect, OnGUI, "FSM State");
    }

    public void DrawArrow()
    {
        if (m_Arrows == null)
            return;
        foreach (EditorArrow arrow in m_Arrows)
        {
            if (arrow == null) continue;
            arrow.Draw(arrow == m_Editor.m_SelectedArrow ? Color.cyan : Color.white);
        }
    }

    public void UpdateCollision()
    {
        FSMStateAction state = m_Controller.GetState(m_StateId) as FSMStateAction;
        if (state == null)
            return;
        if (state.Transitions != null && state.Transitions.Count > 0)
        {
            for (int i = 0; i < m_Arrows.Count; ++i)
            {
                EditorArrow arrow = m_Arrows[i];
                if (arrow == null)
                    continue;
                if (arrow.Contains(Event.current.mousePosition) && !state.Rect.Contains(Event.current.mousePosition))
                {
                    m_Editor.SelectTransition(arrow, state.GetId(), state.Transitions[i]);
                    m_Editor.RepaintView();
                    return;
                }
            }
        }
    }

    public void OnGUI(int id)
    {
        GUI.DragWindow();

        FSMStateAction state = m_Controller.GetState(m_StateId) as FSMStateAction;

        GUILayout.Label(state.StateName);

        string text = string.Empty;
        if (state.Id == m_Controller.DefaultState)
        {
            text = "默认状态\t";
        }
        if (state.Id == m_Controller.GetCurrentStateId())
        {
            text += "正在进行";
        }

        if (text != string.Empty)
        {
            GUILayout.Label(text);
        }

        if (Event.current.type == EventType.used)
        {
            if (Event.current.button == 1)  //弹出菜单
            {
                List<string> list = new List<string>();
                list.Add("Make Transition");
                list.Add("Set As Default State");
                list.Add("Delete");
                EditorTools.CreateMenuWithTextList(list, OnContextMenu);
            }
            else if (Event.current.button == 0) //选中
            {
                if (m_Editor.m_BeginToMakeTransition)
                {
                    m_Editor.EndTransition(state.GetId());
                }
                else
                {
                    m_Editor.SetSelectedState(m_StateId);
                }
            }
        }

        if (state.Transitions == null)
            state.Transitions = new List<FSMTransition>();

        if (m_Arrows == null || state.Transitions.Count != m_Arrows.Count)
        {
            if (state.Transitions != null)
            {
                if (state.Transitions.Count > 0)
                {
                    m_Arrows = new List<EditorArrow>();
                    for (int i = 0; i < state.Transitions.Count; ++i)
                    {
                        FSMStateAction target = m_Controller.GetState(state.Transitions[i].TargetStateId) as FSMStateAction;
                        if (target == null)
                            continue;
                        m_Arrows.Add(new EditorArrow());
                    }
                }
                else
                {
                    m_Arrows.Clear();
                }
            }
        }

        if (state.Transitions != null && state.Transitions.Count > 0)
        {
            for (int i = 0; i < state.Transitions.Count; ++i)
            {
                FSMStateAction target = m_Controller.GetState(state.Transitions[i].TargetStateId) as FSMStateAction;
                if (target != null)
                {
                    //把线条整体往右偏一点，避免与对面的重叠在一起
                    Vector3 dir = target.Rect.center - state.Rect.center;
                    Vector2 dir2 = Vector3.Cross(dir, Vector3.forward).normalized * 6;
                    m_Arrows[i].StartPos = state.Rect.center + dir2;
                    m_Arrows[i].EndPos = target.Rect.center + dir2;
                }
            }
        }
    }

    void OnContextMenu(int index)
    {
        if (index == 0)
        {
            FSMStateAction state = m_Controller.GetState(m_StateId) as FSMStateAction;
            m_Editor.BeginMakeATransition(state.Rect.center, state.GetId());
        }
        else if (index == 1)
        {
            m_Controller.SetDefaultState(m_StateId);
        }
        else if (index == 2)
        {
            m_Controller.RemoveState(m_StateId);
            m_Editor.DeleteWindow(m_WindowId);
        }
    }

}

public class EditorArrow
{
    private static float HalfWidth = 5;

    private Vector3 m_StartPos;
    private Vector3 m_EndPos;

    private Vector2[] m_Polygon = new Vector2[4];

    public Vector3 StartPos
    {
        set
        {
            m_StartPos = value;
            CalcPolygon();
        }
        get { return m_StartPos; }
    }

    public Vector3 EndPos
    {
        set
        {
            m_EndPos = value;
            CalcPolygon();
        }
        get { return m_EndPos; }
    }

    public void Draw(Color color)
    {
        Handles.color = color;
        Handles.DrawLine(StartPos, EndPos);
        Vector3 dir = StartPos - EndPos;
        Vector3 centerPos = (EndPos - StartPos) * 0.5f + StartPos;
        Vector3 p1 = MathEx.RotateAroundZAxis(dir, 30);
        Handles.DrawLine(centerPos, centerPos + p1.normalized * 12);
        p1 = MathEx.RotateAroundZAxis(dir, -30);
        Handles.DrawLine(centerPos, centerPos + p1.normalized * 12);

        CalcPolygon();
        //画出箭头的Obb包围盒
        //Handles.DrawLine(m_Polygon[0], m_Polygon[1]);
        //Handles.DrawLine(m_Polygon[1], m_Polygon[2]);
        //Handles.DrawLine(m_Polygon[2], m_Polygon[3]);
        //Handles.DrawLine(m_Polygon[3], m_Polygon[0]);
    }

    private void CalcPolygon()
    {
        Vector3 dir = EndPos - StartPos;
        dir = Vector3.Cross(dir, Vector3.forward).normalized * HalfWidth;
        m_Polygon[0] = m_StartPos + dir;
        m_Polygon[1] = m_EndPos + dir;
        m_Polygon[2] = m_EndPos - dir;
        m_Polygon[3] = m_StartPos - dir;
    }

    public bool Contains(Vector2 pos)
    {
        return MathEx.PolygonContainsPoint(m_Polygon, pos);
    }
}