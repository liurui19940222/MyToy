using System;
using System.Collections.Generic;
using System.Reflection;
using UnityEngine;

public class Util
{
    //反射调用成员方法
    public static void Invoke(object obj, string method, params object[] param)
    {
        MethodInfo info = obj.GetType().GetMethod(method, BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.Public);
        if (info != null)
        {
            info.Invoke(obj, param);
        }
    }

    //得到枚举最大值
    public static int GetEnumMaxValue(Type type)
    {
        int Max = int.MinValue;

        foreach (int i in Enum.GetValues(type))
        {
            if (i > Max) Max = i;
        }

        return Max;
    }

    //分割Rect
    public static List<Rect> SplitRect(Rect rect, List<float> weights)
    {
        List<Rect> list = new List<Rect>();
        float sum = 0;
        weights.ForEach((f) =>
        {
            sum += f;
        });
        float totalWidth = rect.width;
        float totalHeight = rect.height;
        float x = rect.xMin;
        float y = rect.y;
        for (int i = 0; i < weights.Count; ++i)
        {
            weights[i] /= sum;
            Rect r = new Rect();
            r.width = totalWidth * weights[i];
            r.height = totalHeight;
            r.x = x;
            r.y = y;
            x += r.width * 0.5f;
            x += r.width * 0.5f;
            list.Add(r);
        }

        return list;
    }
}

