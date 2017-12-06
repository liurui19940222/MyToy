using System;
using UnityEngine;

public class Hand : IPartOfBody
{
    private IEquipment m_SecondEquipment;   //副装备

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

    public override IEquipment Equipment
    {
        get
        {
            return base.Equipment;
        }

        set
        {
            base.Equipment = value;
            if (value.Transform)
            {
                value.Transform.SetParent(Transform);
                value.Transform.localScale = Vector3.one;
                value.Transform.localPosition = Vector3.zero;
                value.Transform.localRotation = Quaternion.identity;
                value.GameObject.SetActive(true);
            }
        }
    }

    public IEquipment SecondEquipment
    {
        get { return m_SecondEquipment; }
        set {
            m_SecondEquipment = value;
            m_SecondEquipment.GameObject.SetActive(false);
        }
    }

    /// <summary>
    /// 交换副武器和主武器
    /// </summary>
    public void ChangeEquipmentWithBack()
    {
        if (SecondEquipment == null)
            return;
        IEquipment equip = Equipment;
        Equipment = SecondEquipment;
        SecondEquipment = equip;
    }
}

