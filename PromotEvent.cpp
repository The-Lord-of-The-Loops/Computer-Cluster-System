#include "PromotEvent.h"

void PromotEvent::Execute()
{
	PromotIntertoSys(ID);
}

void PromotEvent::PromotIntertoSys(int ID)
{
	Process tempProcess;
	LinkedQueue<Process> Temp;
	while (!MN->InterWaitingList.isEmpty())
	{
		MN->InterWaitingList.dequeue(tempProcess);
		if (ID != tempProcess.GetID())
			Temp.enqueue(tempProcess);
		else
			MN->SysWaitingList.enqueue(tempProcess, tempProcess.GetPriority());
	}
	while (!Temp.isEmpty())
	{
		Temp.dequeue(tempProcess);
		MN->InterWaitingList.enqueue(tempProcess);
	}
}
