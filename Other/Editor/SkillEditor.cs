using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEditorInternal;

public class SkillEditor : EditorWindow
{

    SkillConfig m_Config;

    ReorderableList m_ConditionList = null;
    ReorderableList m_EventList = null;
    ReorderableList m_TransitionList = null;

    [MenuItem("Tools/Skill Editor")]
    public static void OpenWindow()
    {
        SkillEditor window = GetWindow<SkillEditor>();
        window.minSize = new Vector2(512.0f, 676.0f);
        window.Show();
    }

    void OnGUI()
    {
        EditorTools.DrawTitle("技能属性");
        SkillConfig config = EditorGUILayout.ObjectField("拖放技能配置至此", m_Config, typeof(SkillConfig), true) as SkillConfig;
        if (config == null)
            return;
        if (config != m_Config)
        {
            m_Config = config;
            CreateConditionList();
            CreateEventList();
            CreateTransitionList();
        }
        if (m_ConditionList == null)
            CreateConditionList();
        if (m_EventList == null)
            CreateEventList();
        if (m_TransitionList == null)
            CreateTransitionList();

        m_Config.Id = EditorGUILayout.IntField("技能Id", m_Config.Id);
        m_Config.Name = EditorGUILayout.TextField("技能名字", m_Config.Name);
        m_Config.Desc = EditorGUILayout.TextField("技能描述", m_Config.Desc, GUILayout.Height(50));
        m_Config.ToStiff = EditorGUILayout.Toggle("是否硬直", m_Config.ToStiff);
        if (m_ConditionList != null)
            m_ConditionList.DoLayoutList();

        EditorGUILayout.Space();
        EditorTools.DrawTitle("技能跳转");
        if (m_TransitionList != null)
            m_TransitionList.DoLayoutList();

        EditorGUILayout.Space();

        EditorTools.DrawTitle("技能操作");
        if (m_EventList != null)
            m_EventList.DoLayoutList();

        EditorUtility.SetDirty(m_Config);
    }

    void CreateConditionList()
    {
        m_ConditionList = new ReorderableList(m_Config.Conditions, typeof(ESkillCondition));

        m_ConditionList.drawHeaderCallback = (rect) =>
        {
            EditorGUI.LabelField(rect, "前置条件");
        };

        m_ConditionList.drawElementCallback = (rect, index, isActive, isFocused) =>
        {
            rect.height -= 4;
            rect.y += 2;
            List<float> weights = new List<float>();
            weights.Add(0.3f);
            weights.Add(0.3f);
            weights.Add(0.1f);
            weights.Add(0.3f);
            List<Rect> rects = Util.SplitRect(rect, weights);
            EditorGUI.LabelField(rects[0], "  " + SkillConst.SkillConditionDesc[(int)m_Config.Conditions[index].Condition]);
            m_Config.Conditions[index].Condition = (ESkillCondition)EditorGUI.EnumPopup(rects[1], (ESkillCondition)m_Config.Conditions[index].Condition);
            EditorGUI.LabelField(rects[2], " 参数");
            m_Config.Conditions[index].Param = EditorGUI.TextField(rects[3], m_Config.Conditions[index].Param);
        };

        m_ConditionList.onAddCallback += (list) =>
        {
            m_Config.Conditions.Add(new SkillCondition(ESkillCondition.None));
        };
    }

    void CreateTransitionList()
    {
        m_TransitionList = new ReorderableList(m_Config.Transitions, typeof(SkillTransition));

        m_TransitionList.drawHeaderCallback = (rect) =>
        {
            EditorGUI.LabelField(rect, "转换条件");
        };

        m_TransitionList.drawElementCallback = (rect, index, isActive, isFocused) =>
        {
            rect.height -= 4;
            rect.y += 2;
            List<float> weights = new List<float>();
            weights.Add(0.2f);
            weights.Add(0.3f);
            weights.Add(0.2f);
            weights.Add(0.3f);
            List<Rect> rects = Util.SplitRect(rect, weights);
            EditorGUI.LabelField(rects[0], "输入");
            m_Config.Transitions[index].Input = (EInputWord)EditorGUI.EnumPopup(rects[1], (EInputWord)m_Config.Transitions[index].Input);
            EditorGUI.LabelField(rects[2], " 跳转到");
            m_Config.Transitions[index].TargetSkillId = EditorGUI.IntField(rects[3], m_Config.Transitions[index].TargetSkillId);
        };

        m_TransitionList.onAddCallback += (list) =>
        {
            m_Config.Transitions.Add(new SkillTransition());
        };
    }

    void CreateEventList()
    {
        m_EventList = new ReorderableList(m_Config.Events, typeof(SkillEvent));

        m_EventList.drawHeaderCallback = (rect) =>
        {
            EditorGUI.LabelField(rect, "技能事件");
        };

        m_EventList.drawElementCallback = (rect, index, isActive, isFocused) =>
        {
            rect.height -= 4;
            rect.y += 2;
            List<float> weights = new List<float>();
            weights.Add(0.08f);
            weights.Add(0.12f);
            weights.Add(0.1f);
            weights.Add(0.25f);
            weights.Add(0.25f);
            weights.Add(0.1f);
            weights.Add(0.2f);
            int i = 0;
            List<Rect> rects = Util.SplitRect(rect, weights);
            EditorGUI.LabelField(rects[i++], "时间");
            m_Config.Events[index].DelayTime = EditorGUI.FloatField(rects[i++], m_Config.Events[index].DelayTime);
            EditorGUI.LabelField(rects[i++], " 操作");
            m_Config.Events[index].ActionType = (ESkillActionType)EditorGUI.EnumPopup(rects[i++], (ESkillActionType)m_Config.Events[index].ActionType);
            EditorGUI.LabelField(rects[i++], SkillConst.SkillActionTypeDesc[(int)m_Config.Events[index].ActionType]);
            EditorGUI.LabelField(rects[i++], " 参数");
            m_Config.Events[index].Params = EditorGUI.ObjectField(rects[i++], m_Config.Events[index].Params, typeof(Object), true);
        };

        m_EventList.onAddCallback += (list) =>
        {
            m_Config.Events.Add(new SkillEvent());
        };
    }
}
