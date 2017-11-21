using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class MessageProxy {

    public static void SendSwitchSceneMsg(int sceneId)
    {
        MessageCenter.Instance.SendMessage(MessageDefines.MSG_SWITCH_SCENE, new MSGSwitchScene(sceneId));
    }
}
