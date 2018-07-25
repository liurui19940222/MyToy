using Framework.Common;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace Game
{
    public class ResourceLoader : IResourceLoader
    {
        private const string UI_PATH = "UI/";

        private Dictionary<string, Object> m_LoadedAssets;

        public ResourceLoader()
        {
            m_LoadedAssets = new Dictionary<string, Object>();
        }

        public override Object LoadAsset(string path, string name)
        {
            string key = path + name;
            if (m_LoadedAssets.ContainsKey(key))
            {
                return m_LoadedAssets[key];
            }
            Object obj = Resources.Load(key);
            m_LoadedAssets.Add(key, obj);
            return obj;
        }

        public override Object LoadUIAsset(string assetName)
        {
            return LoadAsset(UI_PATH, assetName);
        }
    }
}
