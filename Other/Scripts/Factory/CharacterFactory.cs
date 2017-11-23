using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CharacterFactory {

    public T CreateCharacter<T>(int id) where T : ICharacter, new()
    {
        CharacterConfig ch_cfg = ResourceFactory.Instance.LoadCharacterConfig(id);
        Object prefab = ResourceFactory.Instance.LoadCharacter(ch_cfg.PrefabName);
        GameObject go = GameObject.Instantiate(prefab) as GameObject;
        T character = new T();
        character.GameObject = go;
        character.Transform = go.transform;
        character.Config = ch_cfg;
        character.Animator = go.GetComponentInChildren<Animator>();
        character.Controller = go.GetComponentInChildren<CharacterController>();
        character.AnimBehaviour = character.Animator.gameObject.AddComponent<AnimationBehaviour>();
        character.AnimBehaviour.StepVoiceSound = ch_cfg.StepVoiceSound;
        character.OnInitialize();
        return character;
    }

    public IPartOfBody[] CreateBody()
    {
        IPartOfBody[] body = new IPartOfBody[Util.GetEnumMaxValue(typeof(EPartOfBodyType)) + 1];
        body[(int)EPartOfBodyType.Head] = new IPartOfBody();
        body[(int)EPartOfBodyType.Chest] = new IPartOfBody();
        body[(int)EPartOfBodyType.Back] = new IPartOfBody();
        body[(int)EPartOfBodyType.Shoulders] = new IPartOfBody();
        body[(int)EPartOfBodyType.Arms] = new IPartOfBody();
        body[(int)EPartOfBodyType.Waist] = new IPartOfBody();
        body[(int)EPartOfBodyType.LeftHand] = new Hand();
        body[(int)EPartOfBodyType.RightHand] = new Hand();
        body[(int)EPartOfBodyType.Legs] = new IPartOfBody();
        body[(int)EPartOfBodyType.Foots] = new IPartOfBody();
        return body;
    }
}
