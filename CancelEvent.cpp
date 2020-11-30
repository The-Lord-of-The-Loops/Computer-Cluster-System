#include "CancelEvent.h"

CancelEvent::CancelEvent(int AT, int ID) : Event(AT, ID) {}

void CancelEvent::deletSysProcess(int ID, LinkedList<Process> &Sys)
{
	Process tempProcess;
	PriorityQueue<Process> Temp;
	while (!Sys.isEmpty())
	{
		Sys.DeleteFirst();
		if (ID != tempProcess.GetID())
			Temp.enqueue(tempProcess, tempProcess.GetPriority());
	}
	while (!Temp.isEmpty())
	{
		Temp.dequeue(tempProcess);
		Sys.InsertSorted(tempProcess, tempProcess.GetPriority());
	}
}

void CancelEvent::Execute(LinkedList<Process> &Sys, LinkedQueue<Process> &Inter, LinkedQueue<Process> &Comp)
{
	deletSysProcess(ID, Sys);
}
