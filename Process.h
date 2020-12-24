#pragma once

#include "Machine.h"

enum ProcessType{
    System,
    Interactive,
    ComputationallyIntensive
};
enum ProcessStatus {
    Complete,
    Waiting,
    Dispatched,
    InExecution
};
class Process {
private:
    int ArrivalTime , ID;
    ProcessType Type;
    ProcessStatus Status;
    int DispatchLatency, ExecutionTime, Priority;
public:
    Process();
    Process(int ArrivalTime, int ID, ProcessType Type,int DispatchLatency, int ExecutionTime, int Priority);
    void operator=(const Process& process)
    {
        ArrivalTime = process.ArrivalTime;
        Type = process.Type;
        Status = process.Status;
        DispatchLatency = process.DispatchLatency;
        ExecutionTime = process.ExecutionTime;
        Priority = process.Priority;
        ID = process.ID;
        WT = process.WT;
        AssignedLastCycle = process.AssignedLastCycle;
        AssignedMachineID = process.AssignedMachineID;
        AssignedMachineType = process.AssignedMachineType;
    }
    //Setters
    int WT = 0;
    void SetArrivalTime(int ArrivalTime);
    void SetProcessType(ProcessType Type);
    void SetDispatchLatency(int DispatchLatency);
    void SetExecutionTime(int ExecutionTime);
    void SetPriority(int Priority);
    void SetStatus(ProcessStatus Status);
    //Getters
    int GetArrivalTime();
    ProcessType GetProcessType();
    int GetDispatchLatency();
    int GetExecutionTime();
    int GetPriority();
    int GetID();
    ProcessStatus getStatus();
    void NotAssignedLastCycle();
    bool AssignedLastCycle = false;
    int AssignedMachineID;
    MachineType AssignedMachineType;
    int getCT();

    ~Process();
};
