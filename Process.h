#pragma once

#include "Machine.h"

enum ProcessType
{
    System,
    Interactive,
    ComputationallyIntensive
};

enum ProcessStatus
{
    Complete,
    Waiting,
    Dispatched,
    InExecution
};

class Process
{
private:
    int ArrivalTime, ID;
    ProcessType Type;
    ProcessStatus Status;
    int DispatchLatency, ExecutionTime, Priority;
    static int intertot;

public:
    Process();
    Process(int ArrivalTime, int ID, ProcessType Type, int DispatchLatency, int ExecutionTime, int Priority);
    void operator=(const Process &process)
    {
        ArrivalTime = process.ArrivalTime;
        Type = process.Type;
        Status = process.Status;
        DispatchLatency = process.DispatchLatency;
        ExecutionTime = process.ExecutionTime;
        Priority = process.Priority;
        ID = process.ID;
        WT = process.WT;
        Executionpriority = process.Executionpriority;
        Assigncycle = process.Assigncycle;
        AssignedLastCycle = process.AssignedLastCycle;
        AssignedMachineID = process.AssignedMachineID;
        AssignedMachineType = process.AssignedMachineType;
    }
    //Setters
    int WT = 0, Assigncycle = 0, Executionpriority = 0;
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
    static int Getinter();
    ProcessStatus getStatus();

    ////// what are these?
    void NotAssignedLastCycle();
    bool AssignedLastCycle = false;
    int AssignedMachineID;
    MachineType AssignedMachineType;
    
    // Completion time
    int getCT();

    ~Process();
};
