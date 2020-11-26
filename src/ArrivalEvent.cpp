#include "ArrivalEvent.h"

ArrivalEvent::ArrivalEvent(int AT, int ID, ProcessType TYP, int DL, int ET, int P, MasterNode* MN): Event(AT,ID, MN)
{
	this->DL = DL;
	this->TYP = TYP;
	this->ET = ET;
	this->P = P;
}

void ArrivalEvent::Execute()
{
	Process NewProcess(ArrivalTime, TYP, DL, ET, P);
	AddtoWaitList(NewProcess);
}

void ArrivalEvent::AddtoWaitList(Process process)
{
	if (process.GetProcessType() == System)
		MN->SysWaitingList.enqueue(process, process.GetPriority());
	else if (process.GetProcessType() == Interactive)
		MN->InterWaitingList.enqueue(process);
	else
		MN->CompIntenWaitingList.enqueue(process);
}


