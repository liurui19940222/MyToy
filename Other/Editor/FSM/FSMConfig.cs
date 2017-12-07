using UnityEngine;
using System.Collections.Generic;

[CreateAssetMenu(fileName = "fsm_cfg", menuName = "配置/状态机设置")]
public class FSMConfig : ScriptableObject {

    public List<FSMParameter> GlobalParamaters;         //公共状态机参数

}
