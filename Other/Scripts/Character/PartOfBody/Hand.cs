using System;
using UnityEngine;

public class Hand : IPartOfBody
{
    public override HandleInputResult HandleInput(EInputWord word, bool down)
    {
        if (m_Euipment != null && down)
        {
            HandleInputResult result = new HandleInputResult();
            if (m_Euipment.Type == EEquipmentType.Shield)
            {
                result.ResultType = HandleInputResult.Type.Sheild;
            }
            else
            {
                result.ResultType = HandleInputResult.Type.Skill;
                if (word == EInputWord.LB || word == EInputWord.RB)
                    result.TargetSkillId = m_Euipment.FirstSkill;
                else if (word == EInputWord.LT || word == EInputWord.RT)
                    result.TargetSkillId = m_Euipment.FirstHeavySkill;
            }
            return result;
        }
        return null;
    }

    public override IEquipment Euipment
    {
        get
        {
            return base.Euipment;
        }

        set
        {
            base.Euipment = value;
            if (value.Transform)
            {
                value.Transform.SetParent(Transform);
                value.Transform.localScale = Vector3.one;
                value.Transform.localPosition = Vector3.zero;
                value.Transform.localRotation = Quaternion.identity;
            }
        }
    }
}

