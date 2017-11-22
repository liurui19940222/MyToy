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
        character.AnimBehaviour = character.Animator.gameObject.AddComponent<AnimationBehaviour>();
        character.AnimBehaviour.StepVoiceSound = ch_cfg.StepVoiceSound;
        //character.GameObject.SetActive(false);
        return character;
    }

    public IPartOfBody[] CreateBody()
    {
        IPartOfBody[] body = new IPartOfBody[Util.GetEnumMaxValue(typeof(EPartOfBodyType)) + 1];
        body[(int)EPartOfBodyType.Head] = new IPartOfBody();
        body[(int)EPartOfBodyType.Chest] = new IPartOfBody();
        body[(int)EPartOfBodyType.Back] = new IPartOfBody();
        body[(int)EPartOfBodyType.LeftShoulder] = new IPartOfBody();
        body[(int)EPartOfBodyType.RightShoulder] = new IPartOfBody();
        body[(int)EPartOfBodyType.LeftArm] = new IPartOfBody();
        body[(int)EPartOfBodyType.RightArm] = new IPartOfBody();
        body[(int)EPartOfBodyType.Waist] = new IPartOfBody();
        body[(int)EPartOfBodyType.LeftHand] = new LeftHand();
        body[(int)EPartOfBodyType.RightHand] = new RightHand();
        body[(int)EPartOfBodyType.LeftLeg] = new IPartOfBody();
        body[(int)EPartOfBodyType.RightLeg] = new IPartOfBody();
        body[(int)EPartOfBodyType.LeftFoot] = new IPartOfBody();
        body[(int)EPartOfBodyType.RightFoot] = new IPartOfBody();
        return body;
    }
}
