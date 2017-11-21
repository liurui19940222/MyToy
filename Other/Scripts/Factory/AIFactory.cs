using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class AIFactory
{

    public CharacterFSMMachine CreateControlledAI(ICharacter ch, RPGGame game)
    {
        CharacterFSMMachine fsm = new CharacterFSMMachine(ch, game);
        fsm.AddState(new ControlledIdleState(AIConst.CONTROLLED_AI_STATE_IDLE, ch));   //待机状态
        fsm.AddState(new ControlledShield(AIConst.CONTROLLED_AI_STATE_SHIELD, ch));      //防御状态
        fsm.AddState(new ControlledRelease(AIConst.CONTROLLED_AI_STATE_RELEASE, ch));  //攻击/施法状态
        fsm.SetDefaultState(AIConst.CONTROLLED_AI_STATE_IDLE);
        return fsm;
    }

}

