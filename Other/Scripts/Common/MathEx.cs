using System.Collections;
using UnityEngine;

public class MathEx
{

    //顶点(向量)绕着Y轴旋转angle角度
    public static Vector3 RotateAroundYAxis(Vector3 vector, float angle)
    {
        float theta = angle * Mathf.Deg2Rad;
        float cosTheta = Mathf.Cos(theta);
        float sinTheta = Mathf.Sin(theta);
        float x = cosTheta * vector.x - sinTheta * vector.z;
        float z = sinTheta * vector.x + cosTheta * vector.z;
        vector.x = x;
        vector.z = z;
        return vector;
    }

    //顶点(向量)绕着Z轴旋转angle角度
    public static Vector3 RotateAroundZAxis(Vector3 vector, float angle)
    {
        float theta = angle * Mathf.Deg2Rad;
        float cosTheta = Mathf.Cos(theta);
        float sinTheta = Mathf.Sin(theta);
        float x = cosTheta * vector.x - sinTheta * vector.y;
        float y = sinTheta * vector.x + cosTheta * vector.y;
        vector.x = x;
        vector.y = y;
        return vector;
    }

    //判断一个点是否在一个多边形内
    public static bool PolygonContainsPoint(Vector2[] vertices, Vector2 point)
    {
        bool sameSide = true;
        float lastSide = PointDirWithLine(vertices[vertices.Length - 1], vertices[0], point);
        for (int i = 0; i < vertices.Length - 1; ++i)
        {
            float result = PointDirWithLine(vertices[i], vertices[i + 1], point);
            sameSide = sameSide && SameSide(result, lastSide);
            if (!sameSide)
                return false;
            lastSide = result;
        }
        return sameSide;
    }

    //判断点在线段的那一侧
    public static float PointDirWithLine(Vector2 p0, Vector2 p1, Vector2 point)
    {
        Vector2 v1 = p1 - p0;
        Vector2 v2 = point - p0;
        return v1.x * v2.y - v1.y * v2.x;
    }

    //判断两个数是否在同一侧（是否同时大于0或者同时小于0，或者两者相等）
    public static bool SameSide(float a, float b)
    {
        if (a < 0 && b < 0) return true;
        else if (a > 0 && b > 0) return true;
        else if (a == b) return true;
        return false;
    }

    //判断一个点是否在扇形范围内
    public static bool DetectFanCollision(Vector3 point, Vector3 centerPos, Vector3 dir, float angle, float distance)
    {
        Vector3 direction = point - centerPos;
        if (Vector3.Dot(direction.normalized, dir) < 0)
            return false;
        float _angle = Vector3.Angle(direction, dir);
        return _angle <= angle * 0.5f && direction.magnitude <= distance;
    }
}

