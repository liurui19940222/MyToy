public interface IFSMMachine
{
    void OnInitialize();

    void AddState(IFSMState state);

    IFSMState GetState(int id);

    void RemoveState(int id);

    void SetAsDefaultState(IFSMState state);

    void SetDefaultState(int id);

    void SwitchToState(int id);

    void HandleInput(EFSMInputType input, Message msg);

    void OnUpdate();

    void Exit();
}

public interface IFSMState
{
    IFSMMachine GetController();

    void SetControllder(IFSMMachine fsm);

    int GetId();

    int HandleInput(EFSMInputType input, Message msg);

    void OnEnter();

    void OnExit();

    int OnUpdate();

    bool HasAlready();
}

