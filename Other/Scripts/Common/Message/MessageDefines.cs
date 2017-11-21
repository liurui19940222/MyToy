using System;

public class MessageDefines {

    public const int MSG_SWITCH_SCENE = 0;      //切换场景

    public const int MSG_CHS_ONINPUT = 1001;       //角色系统，接收到控制器输入
    
}

public class MSGSwitchScene : IMessage
{
    private int m_SceneId;

    public MSGSwitchScene(int sceneId)
    {
        SceneId = sceneId;
    }

    public int SceneId
    {
        get
        {
            return m_SceneId;
        }

        set
        {
            m_SceneId = value;
        }
    }
}

public class WorldMessage : Message
{
    public int MessageId;
}