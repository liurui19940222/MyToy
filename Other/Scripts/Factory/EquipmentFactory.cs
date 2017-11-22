using UnityEngine;

public class EquipmentFactory
{

    public IEquipment CreateEquipment(int id)
    {
        IEquipment equipment = new IEquipment(id);
        return equipment;
    }
}
