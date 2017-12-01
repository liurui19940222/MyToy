using UnityEngine;
using System.Collections.Generic;

public class EffectManager : SingletonMono<EffectManager>
{
    protected override void Awake()
    {
        base.Awake();
    }

    /// <summary>
    /// 得到一个特效实例
    /// </summary>
    public GameObject CreateEffect(string name, Vector3 pos, Quaternion rotation, float lifeTime)
    {
        GameObject go = GameObject.Instantiate(ResourceFactory.Instance.LoadEffect(name)) as GameObject;
        Transform trans = go.transform;
        trans.position = pos;
        trans.rotation = rotation;
        if (lifeTime >= 0)
            GameObject.Destroy(go, lifeTime);
        return go;
    }

    /// <summary>
    /// 创建一个特效实例
    /// </summary>
    public GameObject CreateEffect(Transform parent, string name, Vector3 pos, Quaternion rotation, float lifeTime)
    {
        GameObject go = GameObject.Instantiate(ResourceFactory.Instance.LoadEffect(name)) as GameObject;
        Transform trans = go.transform;
        trans.SetParent(parent);
        trans.localPosition = pos;
        trans.localRotation = rotation;
        if (lifeTime >= 0.0f)
            GameObject.Destroy(go, lifeTime);
        return go;
    }

    /// <summary>
    /// 删除一个特效
    /// </summary>
    public void DestroyEffect(GameObject go)
    {
        GameObject.Destroy(go);
    }

    /// <summary>
    /// 创建火花特效
    /// </summary>
    public GameObject CreateSpark(Spark spark, Vector3 pos, Quaternion rotation)
    {
        GameObject go = null;
        switch (spark)
        {
            case Spark.Iron:
                go = CreateEffect("Spark_Iron", pos, rotation, 1.0f);
                break;
            case Spark.Wood:
                break;
            case Spark.Water:
                break;
            case Spark.Soil:
                break;
        }
        return go;
    }

    /// <summary>
    /// 创建溅血效果
    /// </summary>
    public GameObject CreateBlood(Blood blood, Vector3 pos, Quaternion rotation)
    {
        GameObject go = null;
        switch (blood)
        {
            case Blood.Red_Few:
                break;
            case Blood.Red_Normal:
                go = CreateEffect("BloodEffect", pos, rotation, 2.0f);
                break;
            case Blood.Red_Much:
                go = CreateEffect("LargeBloodEffect", pos, rotation, 2.0f);
                break;
        }
        return go;
    }

    public enum Spark
    {
        Iron,
        Wood,
        Water,
        Soil,
    }

    public enum Blood
    {
        Red_Few,
        Red_Normal,
        Red_Much,
    }
}