using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(CharacterConfig))]
public class CharacterEditor : Editor
{
    public override void OnInspectorGUI()
    {
        EditorTools.DrawTitle("基本属性");
        base.OnInspectorGUI();
        CharacterConfig config = serializedObject.targetObject as CharacterConfig;

        EditorGUILayout.Space();
        EditorTools.DrawTitle("默认装备");
        if (config.DefaultEquipments == null | config.DefaultEquipments.Length == 0)
            config.DefaultEquipments = new int[Util.GetEnumMaxValue(typeof(EPartOfBodyType)) + 1];
        for (int i = 0; i < config.DefaultEquipments.Length; ++i)
        {
            config.DefaultEquipments[i] = EditorGUILayout.IntField(((EPartOfBodyType)i).ToString(), config.DefaultEquipments[i]);
        }
        config.LeftHandSecondEquipment = EditorGUILayout.IntField("左手副装备", config.LeftHandSecondEquipment);
        config.RightHandSecondEquipment = EditorGUILayout.IntField("右手副装备", config.RightHandSecondEquipment);
        EditorUtility.SetDirty(config);
    }
}

