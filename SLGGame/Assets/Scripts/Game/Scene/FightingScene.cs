

using System;
using System.Collections;
using Framework.Common.Message;
using Game.Common;
using UnityEngine;
using Game.Component;

namespace Game.Scene
{
    public class FightingScene : BaseScene
    {
        private MapRes m_MapRes;
        private MapRenderer m_MapRenderer;

        public FightingScene() : base(SceneDefines.SCENE_FIGHTING)
        {
        }

        protected override IEnumerator OnInitialize(IMessage param)
        {
            FightingSceneMsg msg = param as FightingSceneMsg;
            if (msg == null)
            {
                yield break;
            }
            m_MapRes = GameManager.Instance.ResLoader.LoadMapAsset(msg.TargetMapName);
            m_MapRenderer = GameManager.Instance.ResLoader.LoadToolComponent<MapRenderer>("MapRenderer");
            m_MapRenderer.MapData = m_MapRes.MapData;
            m_MapRenderer.Camera = GameCamera.Instance.Camera;
            m_MapRenderer.EnableGridDrawing();
        }

        protected override void OnSceneInitialized()
        {
            Debug.Log("Fighting Scene OnSceneInitialized");
            
        }

        protected override void OnSceneUnload()
        {
            Debug.Log("Fighting Scene OnSceneUnload");
        }

        protected override void OnSceneUpdate()
        {
            
        }
    }
}
