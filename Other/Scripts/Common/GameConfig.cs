using UnityEngine;

[CreateAssetMenu(fileName = "game_cfg", menuName = "配置/游戏通用配置")]
public class GameConfig : ScriptableObject
{
    private static GameConfig _instance;
    public static GameConfig Instance
    {
        get {
            if (_instance == null)
            {
                _instance = ResourceFactory.Instance.LoadAsset<GameConfig>("Config/", "game_cfg");
            }
            return _instance;
        }
    }

    public string MainSceneName;
}

