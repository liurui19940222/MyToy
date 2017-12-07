using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEditorInternal;

[CustomEditor(typeof(FSMConfig))]
public class FSMConfigEditor : Editor {

    ReorderableList m_ConditionList = null;

    public override void OnInspectorGUI()
    {
        FSMConfig cfg = serializedObject.targetObject as FSMConfig;
        if (m_ConditionList == null)
        {
            CreateConditionList(cfg);
        }
        if (m_ConditionList != null)
        {
            m_ConditionList.DoLayoutList();
        }
        EditorUtility.SetDirty(cfg);
    }

    void CreateConditionList(FSMConfig cfg)
    {
        m_ConditionList = new ReorderableList(cfg.GlobalParamaters, typeof(List<FSMParameter>));
        m_ConditionList.drawHeaderCallback = (rect) =>
        {
            EditorGUI.LabelField(rect, "GlobalParameters");
        };

        m_ConditionList.drawElementCallback = (rect, index, isActive, isFocused) =>
        {
            rect.height -= 4;
            rect.y += 2;
            List<float> weights = new List<float>();
            weights.Add(0.475f);
            weights.Add(0.05f);
            weights.Add(0.475f);
            List<Rect> rects = Util.SplitRect(rect, weights);
            cfg.GlobalParamaters[index].Name = EditorGUI.TextField(rects[0], cfg.GlobalParamaters[index].Name);
            if (cfg.GlobalParamaters[index].Value == null)
                cfg.GlobalParamaters[index].Value = new ConditionValue();
            cfg.GlobalParamaters[index].Value.Type = (EValueType)EditorGUI.EnumPopup(rects[2], cfg.GlobalParamaters[index].Value.Type);
        };

        m_ConditionList.onAddCallback += (list) =>
        {
            cfg.GlobalParamaters.Add(new FSMParameter());
        };
    }
}
