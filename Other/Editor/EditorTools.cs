using UnityEngine;
using UnityEditor;

public static class EditorTools
{
    /// <summary>
    /// 绘制一个标题
    /// </summary>
    /// <param name="title"></param>
    public static void DrawTitle(string title)
    {
        EditorGUI.ProgressBar(GUILayoutUtility.GetRect(18, 18, "TextField"), 1.0f, title);
    }

    /// <summary>
    /// 使用枚举创建上下文菜单
    /// </summary>
    /// <typeparam name="T">枚举的类型</typeparam>
    /// <param name="enumType">typeof(枚举)</param>
    /// <param name="callback">选择后的回调</param>
    public static void CreateMenuWithEnum<T>(System.Type enumType, System.Action<int> callback)
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

