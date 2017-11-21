using UnityEngine;
using UnityEditor;

public static class EditorTools
{
    public static void DrawTitle(string title)
    {
        EditorGUI.ProgressBar(GUILayoutUtility.GetRect(18, 18, "TextField"), 1.0f, title);
    }
}

