using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace Game.Scene
{
    public class StartScene : BaseScene
    {
        public StartScene() : base(SceneDefines.SCENE_START)
        {
        }

        protected override void OnSceneInitialized()
        {
            Debug.Log("Start Scene OnSceneInitialized");
        }

        protected override void OnSceneUnload()
        {
            Debug.Log("Start Scene OnSceneUnload");
        }

        protected override void OnSceneUpdate()
        {
            Debug.Log("Start Scene OnSceneUpdate");
        }
    }
}
