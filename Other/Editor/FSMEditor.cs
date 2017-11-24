using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEditorInternal;
using System.Security.Policy;

public class FSMEditor : EditorWindow
{

    FSMController m_Controller;
    Rect m_LeftWindowRect;
    ReorderableList m_ParamsList;

    [MenuItem("Tools/FSM Editor")]
    public static void OpenWindow()
    {
        GetWindow<FSMEditor>().Show();
    }

    void OnEnable()
    {

    }

    void OnGUI()
    {
        BeginWindows();

        m_LeftWindowRect.Set(0, 0, 200, position.height);
        m_LeftWindowRect = GUI.Window(0, m_LeftWindowRect, OnDrawLeftWindow, "条件参数");

        EndWindows();
    }

    void OnDrawLeftWindow(int windowId)
    {
        if (GUILayout.Button("Repaint"))
        {

            Repaint();
        }
        EditorGUILayout.BeginHorizontal();

        GUILayout.Label("拖放");
        m_Controller = EditorGUILayout.ObjectField(m_Controller, typeof(FSMController), true) as FSMController;

        EditorGUILayout.EndHorizontal();

        if (m_Controller == null)
            return;

        if (m_ParamsList == null)
            CreateParamsList();
        m_ParamsList.DoLayoutList();
    }

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
                Debug.Log((EValueType)index);
            });
        };
    }

    void CreateMenu<T>(System.Type enumType, System.Action<int> callback, int index)
    {
        Event evt = Event.current;

        Vector2 mousePos = evt.mousePosition;

        GenericMenu menu = new GenericMenu();

        System.Array array = System.Enum.GetValues(enumType);

        foreach (T i in array)
        {
            menu.AddItem(new GUIContent(i.ToString()), false, () =>
            {
                if (callback != null)
                {
                    callback(System.Convert.ToInt32(i));
                }
            });
        }

        menu.ShowAsContext();

        evt.Use();
    }
}
