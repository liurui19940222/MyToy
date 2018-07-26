using Framework.AStar;
using Framework.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Game.SLG
{
    public class SLGGame : Singleton<SLGGame>
    {
        private GridMap2D m_MapData;
        private SLGCharacterSystem m_CharacterSystem;

        public GridMap2D MapData { get { return m_MapData; } }

        public void Load(GridMap2D mapData)
        {
            m_MapData = mapData;
            m_CharacterSystem = GameManager.Instance.CreateSystem<SLGCharacterSystem>();
        }

        public void UnLoad()
        {

        }
    }
}
