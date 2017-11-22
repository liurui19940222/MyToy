using System;

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
}

