#include "ArrivalEvent.h"

ArrivalEvent::ArrivalEvent(int AT, int ID, ProcessType TYP, int DL, int ET, int P): Event(AT,ID)
{
	this->DL = DL;
	this->TYP = TYP;
	this->ET = ET;
	this->P = P;
}

void ArrivalEvent::Execute(PriorityQueue<Process>& Sys, LinkedQueue<Process>& Inter, LinkedQueue<Process>& Comp)
{
	Process NewProcess(ArrivalTime, TYP, DL, ET, P);
	AddtoWaitList(NewProcess, Sys, Inter, Comp);
}

void ArrivalEvent::AddtoWaitList(Process process, PriorityQueue<Process>& Sys, LinkedQueue<Process> & Inter, LinkedQueue<Process>& Comp)
{
	if (process.GetProcessType() == System)
		Sys.enqueue(process, process.GetPriority());
	else if (process.GetProcessType() == Interactive)
		Inter.enqueue(process);
	else
		Comp.enqueue(process);
}


