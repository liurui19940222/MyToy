using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Test : MonoBehaviour
{

    public Transform[] polygon;

    public Transform point;

    void Start()
    {

    }

    void Update()
    {
        Vector2[] ver = new Vector2[polygon.Length];
        for (int i = 0; i < ver.Length; i++)
        {
            ver[i] = polygon[i].position;
        }
        Debug.Log(MathEx.PolygonContainsPoint(ver, point.position));
    }

    void OnGUI()
    {
        if (GUILayout.Button("button"))
        {

        }
    }


}

