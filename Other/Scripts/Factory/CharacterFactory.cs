using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CharacterFactory {

    public T CreateCharacter<T>(string config) where T : ICharacter, new()
    {
        CharacterConfig ch_cfg = ResourceFactory.Instance.LoadCharacterConfig(config);
        Object prefab = ResourceFactory.Instance.LoadCharacter(ch_cfg.PrefabName);
        GameObject go = GameObject.Instantiate(prefab) as GameObject;
        T character = new T();
        character.GameObject = go;
        character.Transform = go.transform;
        character.Config = ch_cfg;
        character.Animator = go.GetComponentInChildren<Animator>();
        character.Controller = go.GetComponentInChildren<CharacterController>();
        AnimationBehaviour behav = character.Animator.gameObject.AddComponent<AnimationBehaviour>();
        behav.StepVoiceSound = ch_cfg.StepVoiceSound;
        character.GameObject.SetActive(false);
        return character;
    }
}
