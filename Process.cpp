#include "Process.h"

Process::Process()
{
}

Process::Process(int ArrivalTime, ProcessType Type, int DispatchLatency, int ExecutionTime, int Priority) {
this-> ArrivalTime = ArrivalTime;
this-> Type = Type;
this-> DispatchLatency = DispatchLatency;
this-> ExecutionTime = ExecutionTime;
this-> Priority = Priority;
}

void Process::SetArrivalTime(int ArrivalTime) {
this-> ArrivalTime = ArrivalTime;
}

void Process::SetProcessType(ProcessType Type) {
this-> Type = Type;
}

void Process::SetDispatchLatency(int DispatchLatency) {
this-> DispatchLatency = DispatchLatency;
}

void Process::SetExecutionTime(int ExecutionTime) {
this-> ExecutionTime = ExecutionTime;
}

void Process::SetPriority(int Priority) {
this-> Priority = Priority;
}

void Process::SetStatus(ProcessStatus Status)
{
    this->Status = Status;
}


int Process::GetArrivalTime() {
    return ArrivalTime;
}

ProcessType Process::GetProcessType() {
    return Type;
}

int Process::GetDispatchLatency() {
    return DispatchLatency;
}

int Process::GetExecutionTime() {
    return ExecutionTime;
}

int Process::GetPriority() {
    return Priority;
}

int Process::GetID()
{
    return ID;
}

ProcessStatus Process::getStatus()
{
    return Status;
}


Process::~Process()
{
}
