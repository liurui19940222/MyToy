using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MonoScript : SingletonMono<MonoScript> {

    public Coroutine DoCoroutine(IEnumerator cor)
    {
        return StartCoroutine(cor);
    }
}
