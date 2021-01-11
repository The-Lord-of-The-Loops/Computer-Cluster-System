#include "ArrivalEvent.h"

ArrivalEvent::ArrivalEvent(int AT, int ID, ProcessType TYP, int DL, int ET, int P): Event(AT,ID)
{
	this->DL = DL;
	this->TYP = TYP;
	this->ET = ET;
	this->P = P;
}

void ArrivalEvent::Execute(LinkedList<Process>& Sys, LinkedList<Process>& Inter, LinkedQueue<Process>& Comp)
{
	Process NewProcess(ArrivalTime, ID, TYP, DL, ET, P);
	AddtoWaitList(NewProcess, Sys, Inter, Comp);
}

void ArrivalEvent::AddtoWaitList(Process process, LinkedList<Process>& Sys, LinkedList<Process> & Inter, LinkedQueue<Process>& Comp)
{
	if (process.GetProcessType() == System)
		Sys.InsertSorted(process, process.GetPriority());
	else if (process.GetProcessType() == Interactive)
	{
		Inter.InsertLast(process);
	}
	else
		Comp.enqueue(process);
}


