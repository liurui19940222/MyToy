using UnityEngine;

public class EquipmentFactory
{

    public IEquipment CreateEquipment(int id)
    {
        EquipmentConfig config = ResourceFactory.Instance.LoadEquipConfig(id);
        IEquipment equipment = null;
        if (IEquipment.IsWeapon(config.Type))
            equipment = new Weapon(config);
        else
            equipment = new IEquipment(config);
        return equipment;
    }

}
