using System;
using System.Collections.Generic;

public class BattleCharacter : ICharacter
{
    private static IDressingRoom DressingRoom = new DressingRoom();

    private IPartOfBody[] m_Body;           //身体各部分

    public override void OnInitialize()
    {
        base.OnInitialize();
        m_Body = Factory.Instance.CreateBody();
    }

    //穿戴装备
    public void Wear(IEquipment equipment)
    {
        DressingRoom.Wear(this, equipment, IDressingRoom.EBodySide.Left);
    }

    //装备武器
    public void WearWeapon(IEquipment equipment, IDressingRoom.EBodySide side)
    {
        DressingRoom.Wear(this, equipment, side);
    }

    //穿戴装备
    public void Wear(EPartOfBodyType body, IEquipment equipment)
    {
        m_Body[(int)body].Euipment = equipment;
    }

    //根据输入得到处理结果
    public HandleInputResult GetInputResult(EInputWord word, bool down)
    {
        if (word == EInputWord.RB || word == EInputWord.RT)
            return m_Body[(int)EPartOfBodyType.RightHand].HandleInput(word, down);
        else if(word == EInputWord.LB || word == EInputWord.LT)
            return m_Body[(int)EPartOfBodyType.LeftHand].HandleInput(word, down);
        return null;
    }
}


