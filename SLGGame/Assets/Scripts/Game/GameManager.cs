using Framework.Common;
using Framework.FSM;
using Framework.UI;
using Game.Scene;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Game
{
    public class GameManager : Singleton<GameManager>
    {
        private UIManager m_UIManager;

        private ResourceLoader m_ResLoader;

        private FSMMachine m_SceneFSM;

        public UIManager UIMgr { get { return m_UIManager; } }

        public ResourceLoader ResLoader { get { return m_ResLoader; } }

        public void Launch()
        {
            m_ResLoader = new ResourceLoader();
            m_UIManager = new UIManager();
            m_UIManager.OnInitialize(m_ResLoader);

            m_SceneFSM = new FSMMachine();
            m_SceneFSM.AddState(new StartScene());
            m_SceneFSM.SetDefaultState(SceneDefines.SCENE_START);
        }

        public void OnUpdate()
        {
            m_UIManager.OnUpdate();
            m_SceneFSM.OnUpdate();
        }

        public void Quit()
        {
            m_SceneFSM.Quit();
            m_UIManager.OnUninitialize();
        }
    }
}
