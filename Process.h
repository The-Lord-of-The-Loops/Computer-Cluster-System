#pragma once
enum ProcessType{
    System,
    Interactive,
    ComputationallyIntensive
};
class Process {
private:
    float ArrivalTime;
    ProcessType Type;
    int DispatchLatency, ExecutionTime, Priority;
public:
    Process(float ArrivalTime, ProcessType Type,int DispatchLatency, int ExecutionTime, int Priority);
    //Setters
    void SetArrivalTime(float ArrivalTime);
    void SetProcessType(ProcessType Type);
    void SetDispatchLatency(int DispatchLatency);
    void SetExecutionTime(int ExecutionTime);
    void SetPriority(int Priority);
    //Getters
    float GetArrivalTime();
    ProcessType GetProcessType();
    int GetDispatchLatency();
    int GetExecutionTime();
    int GetPriority();

    ~Process();
};
