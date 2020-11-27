#include "CancelEvent.h"

void CancelEvent::deletSysProcess(int ID)
{
	Process tempProcess;
	PriorityQueue<Process> Temp;
	while (!MN->SysWaitingList.isEmpty())
	{
		MN->SysWaitingList.dequeue(tempProcess);
		if (ID != tempProcess.GetID())
			Temp.enqueue(tempProcess, tempProcess.GetPriority());
	}
	while (!Temp.isEmpty())
	{
		Temp.dequeue(tempProcess);
		MN->SysWaitingList.enqueue(tempProcess, tempProcess.GetPriority());
	}
}

void CancelEvent::Execute()
{
	deletSysProcess(ID);
}
